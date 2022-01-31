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

#include "net/un/stream/ConfigConnect.h"

#include "utils/Config.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/CLI11.hpp"

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::un::stream {

    ConfigConnect::ConfigConnect(CLI::App* baseSc) {
        connectSc = baseSc->add_subcommand("bind");
        connectSc->description("Bind options");
        connectSc->configurable();

        connectSunPathOpt = connectSc->add_option("-p,--path", connectSunPath, "Unix domain socket");
        connectSunPathOpt->type_name("[filesystem path]");
        connectSunPathOpt->default_val("/tmp/sun.sock");
        connectSunPathOpt->configurable();
    }

    const std::string& ConfigConnect::getConnectSunPath() const {
        return connectSunPath;
    }

    SocketAddress ConfigConnect::getConnectAddress() const {
        return SocketAddress(connectSunPath);
    }

    void ConfigConnect::required(bool req) const {
        utils::Config::instance().required(connectSc, req);
        utils::Config::instance().required(connectSunPathOpt, req);
    }

} // namespace net::un::stream
