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

#ifndef NET_EVENTRECEIVER_H
#define NET_EVENTRECEIVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <climits>
#include <ctime>
#include <sys/time.h> // for timeval

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core {

    class EventDispatcher;

    class ObservationCounter {
    public:
        bool isObserved() {
            return observationCounter > 0;
        }

    protected:
        int observationCounter = 0;
        struct timeval lastTriggered = {0, 0};
    };

    class EventReceiver : virtual public ObservationCounter {
        EventReceiver(const EventReceiver&) = delete;
        EventReceiver& operator=(const EventReceiver&) = delete;

    protected:
        class TIMEOUT {
        public:
            static const long DEFAULT = -1;
            static const long DISABLE = LONG_MAX;
        };

        explicit EventReceiver(EventDispatcher& descriptorEventDispatcher, long timeout = TIMEOUT::DISABLE);

        virtual ~EventReceiver() = default;

    public:
        void enable(int fd);
        void disable();

        void suspend();
        void resume();

    protected:
        void setTimeout(long timeout);

        bool isEnabled() const;
        bool isSuspended() const;

    private:
        virtual void dispatchEvent() = 0;
        virtual void timeoutEvent();
        virtual void unobservedEvent() = 0;

        virtual bool continueImmediately() = 0;

        void enabled();
        void disabled();

        void suspended();
        void resumed();

        struct timeval getTimeout() const;
        struct timeval getLastTriggered();

        void triggered(struct timeval lastTriggered = {time(nullptr), 0});

        EventDispatcher& descriptorEventDispatcher;

        int fd = -1;

        bool _enabled = false;
        bool _suspended = false;

        long maxInactivity = LONG_MAX;
        const long initialTimeout;

        friend class EventDispatcher;
    };

} // namespace core

#endif // NET_EVENTRECEIVER_H