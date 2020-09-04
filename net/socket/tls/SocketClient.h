/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020  Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TLS_SOCKETCLIENT_H
#define TLS_SOCKETCLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <map>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketClient.h"
#include "socket/tls/SocketConnection.h"

struct SSLDeleter {
    void operator()(SSL* _p) {
        SSL_shutdown(_p);
        SSL_free(_p);
    }

    void operator()([[maybe_unused]] SSL_CTX* _p) {
        if (_p != nullptr) {
            SSL_CTX_free(_p);
        }
    }
};

namespace net::socket::tls {
    class SocketClient : public socket::SocketClient<tls::SocketConnection> {
    public:
        SocketClient(const std::function<void(tls::SocketConnection* socketConnection)>& onConnect,
                     const std::function<void(tls::SocketConnection* socketConnection)>& onDisconnect,
                     const std::function<void(tls::SocketConnection* socketConnection, const char* junk, ssize_t junkLen)>& onRead,
                     const std::function<void(tls::SocketConnection* socketConnection, int errnum)>& onReadError,
                     const std::function<void(tls::SocketConnection* socketConnection, int errnum)>& onWriteError,
                     const std::map<std::string, std::any>& options = {{}});

        ~SocketClient() override;

    protected:
        using socket::SocketClient<tls::SocketConnection>::SocketClient;

    public:
        // NOLINTNEXTLINE(google-default-arguments)
        void connect(const std::map<std::string, std::any>& options, const std::function<void(int err)>& onError,
                     const socket::InetAddress& localAddress = socket::InetAddress()) override;

    private:
        SSL_CTX* ctx = nullptr;
        unsigned long sslErr = 0;
        static int passwordCallback(char* buf, int size, int rwflag, void* u);

        std::function<void(int err)> onError;
    };

} // namespace net::socket::tls

#endif // TLS_SOCKETCLIENT_H
