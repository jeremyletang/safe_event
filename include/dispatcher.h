// The MIT License (MIT)
//
// Copyright (c) 2015 Jeremy Letang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SE_DISPATCHER
#define SE_DISPATCHER

#include <string>
#include <type_traits>
#include <functional>
#include <vector>
#include <iostream>
#include <mutex>

#include "event.h"
#include "typed_map.h"

namespace se {

// internal event listener storage
template<typename E>
struct event_producer {
    // the listeners
    std::vector<std::function<void(const E&)>> _listeners;

    // dispatch the event to the listeners
    void dispatch(const E &ev) {
        for (auto l : this->_listeners) {
            l(ev);
        }
    }

    // add a new listener to the internal vector of listener for this event
    void add_listener(std::function<void(const E&)> fn) {
        this->_listeners.push_back(fn);
    }

    // get the listeners list
    const std::vector<std::function<void(const E&)>> listeners() const {
        return this->_listeners;
    }
};

class dispatcher {

private:

    // list of listeners for all events
    std::vector<std::function<void(const any_event&)>> all_events_listeners;

    // typed_map containing the events producers
    // this ensure that only one producer exist for a given event
    se::typed_map producers;

    // allow us to use the event in an asynchronous way
    std::mutex access_mtx;

public:

    dispatcher();

    ~dispatcher();

    // register an event inside the event dispatcher
    // this function muse be called before the first call of trigger and add_listener
    // for an event, otherwise the event is not known by the event_dispatcher
    // and register event will fail.
    // no parameter is needed, and the event is specified with type parameter
    // usage example: my_ev_dispatcher.register_event<custom_event>();
    // an event can be registered only one time.
    // on success this function create a new event_producer, store it internaly
    // and return true, on error false is returned.
    template<typename E>
    bool add_event() {
        std::lock_guard<std::mutex> lock(this->access_mtx);

        if (this->producers.exist<event_producer<E>>()) {
            return false;
        }

        this->producers.add<event_producer<E>>();
        return true;
    }

    // add a listener to an existing event.
    // the function take an std::function parameterized with the type of the
    // event, e.g for an event `custom_event`, the parameter of add_listener
    // should be of type std::function<void(const custom_event&)>
    // if the event exist true true is return, false otherwise
    template<typename E>
    bool listen(std::function<void(const E&)> fn) {
        std::lock_guard<std::mutex> lock(this->access_mtx);

        if (!this->producers.exist<event_producer<E>>()) {
            this->add_event<E>();
        }

        this->producers.get<event_producer<E>>()->add_listener(fn);

        return true;
    }

    bool listen_any(std::function<void(const any_event&)> fn) {
        std::lock_guard<std::mutex> lock(this->access_mtx);

        this->all_events_listeners.push_back(fn);

        return true;
    }

    // trigger a new event
    // the event must be previously registered with `add_event`
    // the function check if the event is register the event is
    // dispatched to the producer<E> and true is returned, otherwise
    // false is returned
    template<typename E>
    bool trigger(const E &ev) {
        std::lock_guard<std::mutex> lock(this->access_mtx);

        if (!this->producers.exist<event_producer<E>>()) {
            return false;
        }

        // notify listener of the given event
        this->producers.get<event_producer<E>>()->dispatch(ev);

        // notify listeners of all events
        if (!this->all_events_listeners.empty()) {
            auto any_ev = any_event(ev);
            for (auto al : this->all_events_listeners) {
                al(any_ev);
            }
        }
        return true;
    }

    // cleanup all listeners before exit
    void cleanup();

};

}

#endif
