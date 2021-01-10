/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_SOCKET_stream_SOCKETCONNECTOR_H
#define NET_SOCKET_stream_SOCKETCONNECTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "ConnectEventReceiver.h"
#include "socket/Socket.h"

namespace net::socket::stream {

    template <typename SocketConnectionT>
    class SocketConnector
        : public ConnectEventReceiver
        , public SocketConnectionT::Socket {
    public:
        using SocketConnection = SocketConnectionT;
        using Socket = typename SocketConnection::Socket;
        using SocketAddress = typename Socket::SocketAddress;

        SocketConnector(const std::function<void(SocketConnection* socketConnection)>& onConstruct,
                        const std::function<void(SocketConnection* socketConnection)>& onDestruct,
                        const std::function<void(SocketConnection* socketConnection)>& onConnect,
                        const std::function<void(SocketConnection* socketConnection)>& onDisconnect,
                        const std::function<void(SocketConnection* socketConnection, const char* junk, std::size_t junkLen)>& onRead,
                        const std::function<void(SocketConnection* socketConnection, int errnum)>& onReadError,
                        const std::function<void(SocketConnection* socketConnection, int errnum)>& onWriteError,
                        const std::map<std::string, std::any>& options)
            : onConstruct(onConstruct)
            , onDestruct(onDestruct)
            , onConnect(onConnect)
            , onDisconnect(onDisconnect)
            , onRead(onRead)
            , onReadError(onReadError)
            , onWriteError(onWriteError)
            , options(options) {
        }

        SocketConnector() = delete;
        SocketConnector(const SocketConnector&) = delete;

        SocketConnector& operator=(const SocketConnector&) = delete;

        virtual ~SocketConnector() = default;

        virtual void
        connect(const SocketAddress& remoteAddress, const SocketAddress& bindAddress, const std::function<void(int err)>& onError) {
            this->onError = onError;

            errno = 0;

            Socket::open(
                [this, &bindAddress, &remoteAddress, &onError](int errnum) -> void {
                    if (errnum > 0) {
                        onError(errnum);
                        destruct();
                    } else {
                        Socket::bind(bindAddress, [this, &remoteAddress, &onError](int errnum) -> void {
                            if (errnum > 0) {
                                onError(errnum);
                                destruct();
                            } else {
                                int ret = ::connect(Socket::getFd(), &remoteAddress.getSockAddr(), remoteAddress.getSockAddrLen());

                                if (ret == 0 || errno == EINPROGRESS) {
                                    ConnectEventReceiver::enable(Socket::getFd());
                                } else {
                                    onError(errno);
                                    destruct();
                                }
                            }
                        });
                    }
                },
                SOCK_NONBLOCK);
        }

    protected:
        std::function<void(int err)> onError;

        int tryToCompleteConnect() {
            errno = 0;
            int cErrno = -1;
            socklen_t cErrnoLen = sizeof(cErrno);

            int err = getsockopt(Socket::getFd(), SOL_SOCKET, SO_ERROR, &cErrno, &cErrnoLen);

            if (err == 0) {
                if (cErrno != EINPROGRESS) {
                    if (cErrno == 0) {
                        typename SocketAddress::SockAddr localAddress{};
                        socklen_t localAddressLength = sizeof(localAddress);

                        typename SocketAddress::SockAddr remoteAddress{};
                        socklen_t remoteAddressLength = sizeof(remoteAddress);

                        if (getsockname(Socket::getFd(), reinterpret_cast<sockaddr*>(&localAddress), &localAddressLength) == 0 &&
                            getpeername(Socket::getFd(), reinterpret_cast<sockaddr*>(&remoteAddress), &remoteAddressLength) == 0) {
                            socketConnection =
                                new SocketConnection(onConstruct, onDestruct, onRead, onReadError, onWriteError, onDisconnect);

                            socketConnection->setRemoteAddress(SocketAddress(remoteAddress));
                            socketConnection->setLocalAddress(SocketAddress(localAddress));

                            socketConnection->attach(Socket::getFd());
                            socketConnection->SocketConnection::SocketReader::enable(Socket::getFd());

                            SocketConnector::dontClose(true);
                            stream::SocketConnector<SocketConnection>::ConnectEventReceiver::suspend();

                            onError(0);
                            onConnect(socketConnection);
                        } else {
                            onError(errno);
                        }
                    } else {
                        errno = cErrno;
                        onError(errno);
                    }
                }
            } else {
                onError(errno);
            }

            return cErrno;
        }

    private:
        void connectEvent() override {
            if (tryToCompleteConnect() != EINPROGRESS) {
                ConnectEventReceiver::disable();
            }
        }
        void unobserved() override {
            destruct();
        }

        void destruct() {
            delete this;
        }

        std::function<void(SocketConnection* socketConnection)> onConstruct;
        std::function<void(SocketConnection* socketConnection)> onDestruct;
        std::function<void(SocketConnection* socketConnection)> onConnect;
        std::function<void(SocketConnection* socketConnection)> onDisconnect;
        std::function<void(SocketConnection* socketConnection, const char* junk, std::size_t junkLen)> onRead;
        std::function<void(SocketConnection* socketConnection, int errnum)> onReadError;
        std::function<void(SocketConnection* socketConnection, int errnum)> onWriteError;

        SocketConnection* socketConnection = nullptr;

        std::map<std::string, std::any> options;
    };

} // namespace net::socket::stream

#endif // NET_SOCKET_stream_SOCKETCONNECTOR_H