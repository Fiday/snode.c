/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#ifndef NET_RF_STREAM_SERVERSOCKET_H
#define NET_RF_STREAM_SERVERSOCKET_H

#include "net/ServerSocket.h"     // IWYU pragma: export
#include "net/rf/stream/Socket.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstdint> // IWYU pragma: export
#include <functional>
#include <string> // IWYU pragma: export

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::rf::stream {

    class ServerSocket : public net::ServerSocket<net::rf::stream::Socket> {
        using Super = net::ServerSocket<net::rf::stream::Socket>;

    public:
        using net::ServerSocket<net::rf::stream::Socket>::listen;

        void listen(uint8_t channel, int backlog, const std::function<void(const SocketAddress&, int)>& onError);

        void listen(const std::string& address, int backlog, const std::function<void(const SocketAddress&, int)>& onError);

        void
        listen(const std::string& address, uint8_t channel, int backlog, const std::function<void(const SocketAddress&, int)>& onError);
    };

} // namespace net::rf::stream

#endif // NET_RF_STREAM_SERVERSOCKET_H
