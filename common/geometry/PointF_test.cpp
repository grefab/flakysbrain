#include "common/geometry/PointF.h"
#include <fmt/format.h>
#include "3rdparty/catch/catch.hpp"

TEST_CASE("distance") {
    SECTION("horizontal") {
        PointF a(0, 0);
        PointF b(1, 0);
        REQUIRE(distance(a, b) == 1);
    }
    SECTION("vertical") {
        PointF a(0, 0);
        PointF b(0, 1);
        REQUIRE(distance(a, b) == 1);
    }
    SECTION("diagonal") {
        PointF a(0, 0);
        PointF b(-1, -1);
        REQUIRE(distance(a, b) == sqrtf(2.0));
    }
}
