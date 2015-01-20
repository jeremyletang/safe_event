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

#ifndef SE_TYPED_MAP
#define SE_TYPED_MAP

#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace se {

template<typename T>
void deleter(void* d) {
    delete (T*)d;
}

// an map encapsulation to store any type in the same map
// only one instance of each type can be stored inside the map
class typed_map {

private:

    // the map, each type is referenced by it's type_index and cast to void*
    std::unordered_map<std::type_index, std::pair<void *, std::function<void(void*)>>> map;

public:

    ~typed_map() {
        for (auto &it : this->map) {
            it.second.second(it.second.first);
        }
    }

    // add a new type instance inside the map.
    // the first type parameter is the type of the new object to insert in the map
    // the second is the list of types parameters to instanciate the type
    // the methods parameters are the list of variable to pass to the type instanciation
    // return false if instanciation failed, true otherwise
    template<typename T, class ...A>
    bool add(A... args) {
        T *inst = new T(args...);

        if (inst == nullptr) { return false; }
        auto f = deleter<T>;
        map.emplace(std::type_index(typeid(T)), std::make_pair(inst, f));
        return true;
    }

    // get a pointer to the type T instance inside the map
    // return nullptr if the type is not inside the map or a pointer to the type instance
    template<typename T>
    T *get() {
        auto it = this->map.find(std::type_index(typeid(T)));

        if (it == this->map.end()) { return nullptr; }
        return static_cast<T*>(it->second.first);
    }

    // check if an instance of T is already on the map
    template<typename T>
    bool exist()
    { return !(this->map.find(std::type_index(typeid(T))) == this->map.end()); }

    template<typename T, class ...A>
    bool apply_while(std::function<bool(T*, A...)> f, A... args) {
        for (auto it = this->map.begin(); it != this->map.end(); ++it) {
            if (f(static_cast<T*>(it->second.first, args)...) == true) {
                return true;
            }
        }
        return false;
    }

};

}

#endif
