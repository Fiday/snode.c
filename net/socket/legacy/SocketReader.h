#ifndef LEGACY_SOCKETREADER_H
#define LEGACY_SOCKETREADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketReaderBase.h"
#include "socket/legacy/Socket.h"


namespace legacy {

    class SocketReader
        : public SocketReaderBase
        , virtual public legacy::Socket {
    protected:
        SocketReader(const std::function<void(legacy::SocketReader* cs, const char* junk, ssize_t n)>& readProcessor,
                     const std::function<void(int errnum)>& onError)
            : SocketReaderBase(
                  [readProcessor](SocketReaderBase* cs, const char* junk, ssize_t n) -> void {
                      readProcessor(dynamic_cast<legacy::SocketReader*>(cs), junk, n);
                  },
                  onError) {
        }

    private:
        ssize_t recv(char* junk, const ssize_t& junkSize) override;
    };

}; // namespace legacy

#endif // LEGACY_SOCKETREADER_H
