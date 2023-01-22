#include "common/threadsafe/threadsafe.h"
#include <thread>
#include <vector>
#include "3rdparty/catch/catch.hpp"

namespace isotronic {

struct Data {
    int foo = 23;
    std::string bar = "baz";
};

TEST_CASE("store does sth") {
    threadsafe<Data> s(Data{0, "buzz"});

    SECTION("simple stuff") {
        REQUIRE(!s->bar.empty());
        REQUIRE(s->foo == 0);
        s.mut()->foo = 23;
        REQUIRE(s->foo == 23);
    }

    SECTION("single threaded writing") {
        s.mut()->foo = 0;
        for (int i = 0; i <= 100; ++i) {
            s.mut()->foo = i;
        }
        REQUIRE(s->foo == 100);
    }

    SECTION("multi threaded writing") {
        const size_t thread_count = 100;
        std::vector<std::thread> threads;
        threads.reserve(thread_count);
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&s]() { s.atomic([](auto* data) { data->foo += 1; }); }));
        }
        for (auto& thread : threads) {
            thread.join();
        }
        REQUIRE(s->foo == thread_count);
    }
}

TEST_CASE("benchmarks") {
    threadsafe<Data> s(Data{0, "buzz"});

    SECTION("simple") {
        BENCHMARK("cloning") { return s.clone(); };
        BENCHMARK("adding") { s.mut()->foo = 23; };
    }

    SECTION("fancy") {
        BENCHMARK("single threaded adding") {
            for (int i = 0; i <= 100; ++i) {
                s.mut()->foo = i;
            }
            return 2 * s->foo;
        };
        BENCHMARK("multi threaded adding") {
            const size_t thread_count = 100;
            std::vector<std::thread> threads;
            threads.reserve(thread_count);
            for (int i = 0; i < thread_count; ++i) {
                threads.emplace_back(std::thread([&s]() { s.atomic([](auto* data) { data->foo += 1; }); }));
            }
            for (auto& thread : threads) {
                thread.join();
            }
        };
    }
}

}  // namespace isotronic
