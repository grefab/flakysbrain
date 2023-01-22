//
// Created by user on 5/16/20.
//

#pragma once

#include <functional>
#include <shared_mutex>
#include <utility>
#include "common/threadsafe/ReadWriteLocker.h"

namespace isotronic {

// Provides a store for an object of arbitrary class T. The sole purpose of
// this is to exclusively allow thread-safe access to this object.
// This is basically a convenience wrapper around mutex concepts.
//
// This container has been designed to hold protos, esp. configuration or status semantics.
// Therefore, the (presumably most common) read use case is made easy like this:
//
//     message Data {
//         int foo = 1;
//     }
//
//     threadsafe<Data>->foo() == 23;  // probably false
//
// Writing has to be done explicitly by using mut():
//
//     threadsafe<Data>.mut()->set_foo(42);
//     threadsafe<Data>->foo() == 42;  // true
//
// Note that reading and writing in the same expression might cause deadlocks:
//
//     s.mut()->foo = s->foo + 1;  // deadlocks bc writer is not released when reader is acquired
//
// Use atomic() if a write operation depends on an existing value, e.g.
//
//     threadsafe<Data>.atomic([](auto* d){ d->foo = d->foo +1; });
//
// Consider making local copies of data when needing it a lot, e.g. in loops.

template <typename T>
class threadsafe {
    using mutex_t = std::shared_mutex;

public:
    // a threadsafe object cannot be copied or assigned to. it owns the data and the mutex.
    threadsafe() : data_(T()) {}
    threadsafe(threadsafe const&) = delete;
    threadsafe& operator=(threadsafe const& rhs) = delete;
    explicit threadsafe(T&& t) : data_(std::move(t)) {}

    // read access
    ReadLocker<T> operator->() const { return ReadLocker(const_cast<mutex_t&>(m_), data_); }

    // returns a copy of the current content
    T clone() {
        std::shared_lock<mutex_t> lock(m_);
        return data_;
    }

    // write access
    WriteLocker<T> mut() { return WriteLocker(m_, &data_); }
    void set(T const& t) {
        std::unique_lock<mutex_t> lock(m_);
        data_ = t;
    }

    // executes a function within a unique lock. useful for
    // amending values that depend on the current value.
    void atomic(std::function<void(T*)> const& fn) {
        std::unique_lock<mutex_t> lock(m_);
        fn(&data_);
    }

private:
    T data_;
    mutex_t m_;
};

}  // namespace isotronic
