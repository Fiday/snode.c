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

#ifndef NET_IN6_STREAM_CLIENTCONFIG_H
#define NET_IN6_STREAM_CLIENTCONFIG_H

#include "net/ConfigBacklog.h"
#include "net/ConfigBase.h"
#include "net/ConfigConn.h"
#include "net/in6/ConfigLocal.h"
#include "net/in6/ConfigRemote.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string> // for string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::in6::stream {

    class ClientConfig
        : public net::ConfigBase
        , public net::ConfigBacklog
        , public net::in6::ConfigRemote
        , public net::in6::ConfigLocal
        , public net::ConfigConn {
    public:
        explicit ClientConfig(const std::string& name);
    };

} // namespace net::in6::stream

#endif // NET_IN6_STREAM_CLIENTCONFIG_H