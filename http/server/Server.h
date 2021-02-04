#ifndef HTTP_SERVER_SERVERT_H
#define HTTP_SERVER_SERVERT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <cstddef>
#include <functional>
#include <map>
#include <netinet/in.h>
#include <string>

#endif // DOXYGEN_SHOULD_SKIP_THIS

#include "ServerContext.hpp"

namespace http::server {

    template <typename SocketServerT, typename RequestT = Request, typename ResponseT = Response>
    class Server {
    public:
        using SocketServer = SocketServerT;
        using SocketConnection = typename SocketServer::SocketConnection;
        using SocketAddress = typename SocketConnection::SocketAddress;
        using Request = RequestT;
        using Response = ResponseT;

        Server(const std::function<void(SocketConnection*)>& onConnect,
               const std::function<void(Request& req, Response& res)>& onRequestReady,
               const std::function<void(SocketConnection*)>& onDisconnect,
               const std::map<std::string, std::any>& options = {{}})
            : socketServer(
                  [onRequestReady](SocketConnection* socketConnection) -> void { // onConstruct
                      socketConnection->template setContext<ServerContextBase*>(
                          new ServerContext<Request, Response>(socketConnection, onRequestReady));
                  },
                  [](SocketConnection* socketConnection) -> void { // onDestruct
                      socketConnection->template getContext<ServerContextBase*>([](ServerContextBase* serverContext) -> void {
                          delete serverContext;
                      });
                  },
                  [onConnect](SocketConnection* socketConnection) -> void { // onConnect
                      onConnect(socketConnection);
                  },
                  [onDisconnect](SocketConnection* socketConnection) -> void { // onDisconnect
                      onDisconnect(socketConnection);
                  },
                  [](SocketConnection* socketConnection, const char* junk, std::size_t junkSize) -> void { // onRead
                      socketConnection->template getContext<ServerContextBase*>(
                          [&junk, &junkSize](ServerContextBase* serverContext) -> void {
                              serverContext->receiveRequestData(junk, junkSize);
                          });
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onReadError
                      socketConnection->template getContext<ServerContextBase*>([&errnum](ServerContextBase* serverContext) -> void {
                          serverContext->onReadError(errnum);
                      });
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onWriteError
                      socketConnection->template getContext<ServerContextBase*>([&errnum](ServerContextBase* serverContext) -> void {
                          serverContext->onWriteError(errnum);
                      });
                  },
                  options) {
        }

        void listen(unsigned short port, const std::function<void(int err)>& onError) const {
            socketServer.listen(SocketAddress(port), 5, onError);
        }

        void listen(const std::string& ipOrHostname, unsigned short port, const std::function<void(int err)>& onError) const {
            socketServer.listen(SocketAddress(ipOrHostname, port), 5, onError);
        }

    protected:
        SocketServer socketServer;
    };

} // namespace http::server

#endif // HTTP_SERVER_SERVERT_H