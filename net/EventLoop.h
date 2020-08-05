#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <list>
#include <sys/select.h> // for fd_set

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "AcceptEventDispatcher.h"
#include "OutOfBandEventDispatcher.h"
#include "ReadEventDispatcher.h"
#include "TimerEventDispatcher.h"
#include "WriteEventDispatcher.h"

class EventLoop {
private:
    EventLoop();

public:
    static EventLoop* currentLoop() {
        return currentEventLoop;
    }

    ReadEventDispatcher& getReadEventDispatcher() {
        return readEventDispatcher;
    }

    AcceptEventDispatcher& getAcceptEventDispatcher() {
        return acceptEventDispatcher;
    }

    WriteEventDispatcher& getWriteEventDispatcher() {
        return writeEventDispatcher;
    }

    OutOfBandEventDispatcher& getOutOfBandEventDispatcher() {
        return outOfBandEventDispatcher;
    }

    TimerEventDispatcher& getTimerEventDispatcher() {
        return timerEventDispatcher;
    }

    static void init(int argc, char* argv[]); // NOLINT(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
    static void start();
    static void stop();

private:
    static void stoponsig(int sig);

    inline void tick();

    static std::list<EventLoop*> eventLoops;
    static EventLoop* currentEventLoop;

    fd_set readfds{0};
    fd_set writefds{0};
    fd_set exceptfds{0};

    ReadEventDispatcher readEventDispatcher;
    AcceptEventDispatcher acceptEventDispatcher;
    WriteEventDispatcher writeEventDispatcher;
    OutOfBandEventDispatcher outOfBandEventDispatcher;
    TimerEventDispatcher timerEventDispatcher;

    bool running = false;
    bool stopped = true;
    static bool initialized;
};

#endif // MULTIPLEXER_H
