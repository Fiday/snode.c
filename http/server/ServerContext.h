/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef HTTP_SERVER_SERVERCONTEXT_H
#define HTTP_SERVER_SERVERCONTEXT_H

#include "net/socket/stream/SocketProtocol.h"

namespace net::socket::stream {
    class SocketConnectionBase;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <iostream>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace http::server {

    class ServerContext : public net::socket::stream::SocketProtocol {
    public:
        using SocketConnection = net::socket::stream::SocketConnectionBase;

        virtual ~ServerContext() = default;

        void take(const char* junk, std::size_t junkLen);

        void upgrade(ServerContext* newServerContext);

    private:
        bool markedForDelete = false;
    };

} // namespace http::server

#endif // HTTP_SERVER_SERVERCONTEXT_H
