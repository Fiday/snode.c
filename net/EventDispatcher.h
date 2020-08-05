/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020  Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANAGER_H
#define MANAGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <algorithm>
#include <list>
#include <map>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Descriptor.h"
#include "EventReceiver.h"

class EventLoop;

template <typename Event>
class EventDispatcher {
public:
    explicit EventDispatcher(fd_set& fdSet) // NOLINT(google-runtime-references)
        : fdSet(fdSet) {
    }

    EventDispatcher(const EventDispatcher&) = delete;

    EventDispatcher& operator=(const EventDispatcher&) = delete;

private:
    static bool contains(std::list<Event*>& events, Event*& event) {
        typename std::list<Event*>::iterator it = std::find(events.begin(), events.end(), event);

        return it != events.end();
    }

public:
    void enable(Event* event) {
        if (!event->isEnabled() && !EventDispatcher<Event>::contains(enabledEventReceiver, event)) {
            enabledEventReceiver.push_back(event);
            event->observe();
        }
    }

    void disable(Event* event) {
        if (event->isEnabled()) {
            if (EventDispatcher<Event>::contains(enabledEventReceiver, event)) { // stop() on same tick as start()
                enabledEventReceiver.remove(event);
                event->unobserve();
            } else if (!EventDispatcher<Event>::contains(disabledEventReceiver, event)) { // stop() asynchronously
                disabledEventReceiver.push_back(event);
            }
        }
    }

private:
    int getMaxFd() {
        int fd = -1;

        if (!observedEvents.empty()) {
            fd = observedEvents.rbegin()->first;
        }

        return fd;
    }

    void observeEnabledEvents() {
        for (Event* eventReceiver : enabledEventReceiver) {
            int fd = dynamic_cast<Descriptor*>(eventReceiver)->getFd();
            bool inserted = false;
            std::tie(std::ignore, inserted) = observedEvents.insert({fd, eventReceiver});
            if (inserted) {
                FD_SET(fd, &fdSet);
            } else {
                eventReceiver->unobserve();
            }
        }
        enabledEventReceiver.clear();
    }

    void unobserveDisabledEvents() {
        for (Event* eventReceiver : disabledEventReceiver) {
            int fd = dynamic_cast<Descriptor*>(eventReceiver)->getFd();
            FD_CLR(fd, &fdSet);
            observedEvents.erase(fd);
            eventReceiver->unobserve();
            eventReceiver->checkObserved();
        }
        disabledEventReceiver.clear();
    }

    void unobserveObservedEvents() {
        for (auto& [fd, eventReceiver] : observedEvents) {
            disabledEventReceiver.push_back(eventReceiver);
        }
        unobserveDisabledEvents();
    }

protected:
    virtual int dispatch(const fd_set& fdSet, int count) = 0;

    std::map<int, Event*> observedEvents;

private:
    std::list<Event*> enabledEventReceiver;
    std::list<Event*> disabledEventReceiver;

    fd_set& fdSet;

public:
    using EventType = Event;

    friend class EventLoop;
};

#endif // MANAGER_H
