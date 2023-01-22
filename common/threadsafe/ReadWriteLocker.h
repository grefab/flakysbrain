//
// Created by user on 5/16/20.
//

#pragma once

#include <shared_mutex>

namespace isotronic {

template <typename T>
class ReadLocker {
public:
    ReadLocker(std::shared_mutex& m, T const& data) : m_(m), data_(data) {}
    T const* operator->() const { return &data_; }

private:
    std::shared_lock<std::shared_mutex> m_;
    T const& data_;
};

template <typename T>
class WriteLocker {
public:
    WriteLocker(std::shared_mutex& m, T* data) : m_(m), data_(data) {}
    T* operator->() { return data_; }
    T* ptr() { return data_; }

private:
    std::unique_lock<std::shared_mutex> m_;
    T* data_;
};

}  // namespace isotronic
