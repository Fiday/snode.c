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

//#define BACKWARD_HAS_BFD 1
#define BACKWARD_HAS_DW 1
//#define BACKWARD_HAS_DWARF 1

#include "stacktrace.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <backward.hpp>
#include <stdio.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace stacktrace {

    void stacktrace(std::size_t trace_cnt_max) {
        backward::StackTrace st;
        st.load_here(trace_cnt_max);

        backward::Printer p;
        p.snippet = true;
        p.object = false;
        p.color_mode = backward::ColorMode::always;
        p.address = true;
        p.print(st, stderr);
    }

} // namespace stacktrace

/*
#include <execinfo.h>
#include <stdio.h>
...
void* callstack[128];
int i, frames = backtrace(callstack, 128);
char** strs = backtrace_symbols(callstack, frames);
for (i = 0; i < frames; ++i) {
    printf("%s\n", strs[i]);
}
free(strs);
...
 */

/*
class MyException : public std::exception {

    char ** strs;
    MyException( const std::string & message ) {
         int i, frames = backtrace(callstack, 128);
         strs = backtrace_symbols(callstack, frames);
    }

    void printStackTrace() {
        for (i = 0; i < frames; ++i) {
            printf("%s\n", strs[i]);
        }
        free(strs);
    }
};
 */
/*
try {
   throw MyException("Oops!");
} catch ( MyException e ) {
    e.printStackTrace();
}
 */
