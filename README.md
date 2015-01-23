# safe_event
C++ 11/14 type safe light event system

## A simple example
```C++
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
    ev_disp.listen(std::function<void(const hello_event& ev)>([](const hello_event& ev) {
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
```