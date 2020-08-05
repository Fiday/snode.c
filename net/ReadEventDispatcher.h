#ifndef MANAGEDREADER_H
#define MANAGEDREADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <sys/select.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "EventDispatcher.h"
#include "ReadEvent.h"

// IWYU pragma: no_forward_declare Reader

class ReadEventDispatcher : public EventDispatcher<ReadEvent> {
public:
    using EventDispatcher<ReadEvent>::EventDispatcher;

    int dispatch(const fd_set& fdSet, int count) override;
};

#endif // MANAGEDREADER_H
