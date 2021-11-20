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

#ifndef WEB_WEBSOCKET_SERVER_SUBPROTOCOLSELECTOR_H
#define WEB_WEBSOCKET_SERVER_SUBPROTOCOLSELECTOR_H

#include "web/websocket/SubProtocolFactorySelector.h"

namespace web::websocket::server {
    class SubProtocolFactory;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string> // for string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::server {

    class SubProtocolFactorySelector : public web::websocket::SubProtocolFactorySelector<web::websocket::server::SubProtocolFactory> {
    public:
        static SubProtocolFactorySelector* instance();

        static void link(const std::string& subProtocolName, SubProtocolFactory* (*getSubProtocolFactory)());
        static void addSubProtocolSearchPath(const std::string& searchPath);
        static void allowDlOpen();

    private:
        SubProtocolFactorySelector();

        SubProtocolFactorySelector(const SubProtocolFactorySelector&) = delete;

        SubProtocolFactorySelector& operator=(const SubProtocolFactorySelector&) = delete;

        using web::websocket::SubProtocolFactorySelector<web::websocket::server::SubProtocolFactory>::load;

        SubProtocolFactory* load(const std::string& subProtocolName) override;
    };

} // namespace web::websocket::server

#endif // WEB_WEBSOCKET_SERVER_SUBPROTOCOLSELECTOR_H