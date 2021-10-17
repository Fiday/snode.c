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

#ifndef WEB_HTTP_SOCKETCONTEXT_H
#define WEB_HTTP_SOCKETCONTEXT_H

#include "net/socket/stream/SocketContext.h"

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::http {

    class SocketContext : public net::socket::stream::SocketContext {
    public:
        using SocketConnection = net::socket::stream::SocketConnection;

        explicit SocketContext(net::socket::stream::SocketConnection* socketConnection);

        ~SocketContext() override = default;

        SocketContext(const SocketContext&) = delete;
        SocketContext& operator=(const SocketContext&) = delete;

        virtual void sendToPeerCompleted() = 0;
        virtual void terminateConnection() = 0;
    };

} // namespace web::http

#endif // WEB_HTTP_SOCKETCONTEXT_H