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

#ifndef NET_DYNAMICLOADER_H
#define NET_DYNAMICLOADER_H

#include "core/system/dlfcn.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <map>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core {

    class DynamicLoader {
    private:
        struct Library {
            std::string fileName = "";
            std::size_t refCount = 0;
        };

        DynamicLoader() = delete;

        ~DynamicLoader() = delete;

    public:
        static void* dlOpen(const std::string& libFile, int flags);
        static void dlCloseDelayed(void* handle);
        static int dlClose(void* handle);

        template <typename Symbol>
        static Symbol dlSym(void* handle, const std::string& symbol) {
            return reinterpret_cast<Symbol>(core::system::dlsym(handle, symbol.c_str()));
        }

        static char* dlError();

    private:
        static int execDlClose(void* handle);
        static void execDlCloseDeleyed();
        static void execDlCloseAll();

        static std::map<void*, Library> dlOpenedLibraries;
        static std::map<void*, std::size_t> registeredForDlClose;

        friend class EventLoop;
    };

} // namespace core

#endif // NET_DYNAMICLOADER_H