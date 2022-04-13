/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *               2021, 2022 Daniel Flockert
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

#ifndef DATABASE_MARIADB_COMMANDS_MARIADBFETCHROWCOMMAND
#define DATABASE_MARIADB_COMMANDS_MARIADBFETCHROWCOMMAND

#include "database/mariadb/MariaDBCommand.h"

namespace database::mariadb {
    class MariaDBConnection;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <string>

typedef struct st_mysql_res MYSQL_RES;
typedef char** MYSQL_ROW;

// IWYU pragma: no_include "mysql.h"

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace database::mariadb::commands {

    class MariaDBFetchRowCommand : public MariaDBCommand {
    public:
        MariaDBFetchRowCommand(MariaDBConnection* mariaDBConnection,
                               MYSQL_RES* result,
                               const std::function<void(const MYSQL_ROW)>& onRowResult,
                               const std::function<void(const std::string&, unsigned int)>& onError);
        ~MariaDBFetchRowCommand();

        int execute();
        int start() override;
        int cont(int status) override;

        void commandCompleted() override;
        void commandError(const std::string& errorString, unsigned int errorNumber) override;

        bool error() override;

    private:
        MYSQL_RES* result = nullptr;
        MYSQL_ROW row = nullptr;

        std::function<void(const MYSQL_ROW)> onRowResult;
        std::function<void(const std::string&, unsigned int)> onError;
    };

} // namespace database::mariadb::commands

#endif // DATABASE_MARIADB_COMMANDS_MARIADBFETCHROWCOMMAND