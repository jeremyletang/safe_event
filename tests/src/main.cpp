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

#include <safe_event>
#include <iostream>

struct hello_event {
    void say_hello()
    { std::cout << "hello world !" << std::endl; }
};

int
main() {
    se::dispatcher ev_disp;

    // add the hello_event struct to the available events
    ev_disp.add_event<hello_event>();

    // listen hello_event
    ev_disp.listen(std::function<void(const hello_event&)>([](const hello_event&) {
        std::cout << "This is a real hello_event !" << std::endl;;
    }));

    // listen any event
    ev_disp.listen_any([](const se::any_event& ev) {
        // make dynamic test to check the event type
        if (se::is<hello_event>(ev)) {
            std::cout << "any_event is an hello_event" << std::endl;
            auto real_ev = se::into<hello_event>(ev);
            real_ev.say_hello();
        }
    });

    // dispatch an event
    ev_disp.trigger(hello_event());
}