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

#ifndef SE_EVENT
#define SE_EVENT

#include <typeinfo>
#include <iostream>
#include <memory>

namespace se {

struct base_any {

    virtual ~base_any() {}
    virtual const std::type_info& get_typeinfo() =0;

};

template<typename T>
struct real_any: public base_any {

private:

    const T &val;
    const std::type_info& self;

public:

    real_any(const T &val)
    : val(val), self(typeid(T))
    {}

    const std::type_info& get_typeinfo()
    { return self; };

    const T& get()
    { return this->val; }

};

struct any_event {

private:

    std::unique_ptr<base_any> base;

public:

    any_event() {}

    template<typename T> any_event(const T &val)
    : base(std::make_unique<real_any<T>>(val))
    {}

    const std::type_info& get_typeinfo()
    { return this->base->get_typeinfo(); };

    template<typename T> bool is() const
    { return this->base->get_typeinfo() == typeid(T); }

    template<typename T>
    const T& into() const {
        if (!this->is<T>()) { throw std::bad_cast(); }
        auto ptr = dynamic_cast<real_any<T>*>(this->base.get());
        if (ptr == nullptr) { throw std::bad_cast(); }
        return ptr->get();
    }
};

template<typename T>
const T& into(const any_event &ev) {
    return ev.into<T>();
}

template<typename T>
bool is(const any_event& ev) {
    return ev.is<T>();
}

}

#endif