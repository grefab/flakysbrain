#include "common/geometry/LineF.h"
#include <fmt/format.h>
#include "3rdparty/catch/catch.hpp"

TEST_CASE("line intersection") {
    SECTION("intersect in origin") {
        LineF h{PointF(-1, 0), PointF(1, 0)};
        LineF v{PointF(0, -1), PointF(0, 1)};
        auto intersection = lineIntersection(h, v);
        REQUIRE(intersection);
        REQUIRE(*intersection == PointF(0, 0));
    }

    SECTION("lines intersect outside of their segment") {
        LineF h{PointF(-1, 0), PointF(1, 0)};
        LineF v{PointF(2, -1), PointF(2, 1)};
        auto intersection = lineIntersection(h, v);
        REQUIRE(intersection);
        REQUIRE(*intersection == PointF(2, 0));
    }

    SECTION("parallel horizontal lines does not intersect") {
        LineF l1{PointF(0, 0), PointF(1, 0)};
        LineF l2{PointF(0, 1), PointF(1, 1)};
        auto intersection = lineIntersection(l1, l2);
        REQUIRE(!intersection);
    }

    SECTION("parallel 45° lines do not intersect") {
        LineF l1{PointF(0, 0), PointF(1, 1)};
        LineF l2{PointF(0, 1), PointF(1, 2)};
        auto intersection = lineIntersection(l1, l2);
        REQUIRE(!intersection);
    }
}

TEST_CASE("line segment intersection") {
    SECTION("intersect in origin") {
        LineF h{PointF(-1, 0), PointF(1, 0)};
        LineF v{PointF(0, -1), PointF(0, 1)};
        auto intersection = lineSegmentIntersection(h, v);
        REQUIRE(intersection);
        REQUIRE(*intersection == PointF(0, 0));
    }

    SECTION("lines do not intersect outside of their segment") {
        LineF h{PointF(-1, 0), PointF(1, 0)};
        LineF v{PointF(2, -1), PointF(2, 1)};
        auto intersection = lineSegmentIntersection(h, v);
        REQUIRE(!intersection);
    }

    SECTION("parallel horizontal lines does not intersect") {
        LineF l1{PointF(0, 0), PointF(1, 0)};
        LineF l2{PointF(0, 1), PointF(1, 1)};
        auto intersection = lineSegmentIntersection(l1, l2);
        REQUIRE(!intersection);
    }

    SECTION("parallel 45° lines do not intersect") {
        LineF l1{PointF(0, 0), PointF(1, 1)};
        LineF l2{PointF(0, 1), PointF(1, 2)};
        auto intersection = lineSegmentIntersection(l1, l2);
        REQUIRE(!intersection);
    }
}

TEST_CASE("line center") {
    SECTION("intersect in origin") {
        LineF h{PointF(-1, 0), PointF(1, 0)};
        REQUIRE(center(h) == PointF(0, 0));
        LineF v{PointF(0, -1), PointF(0, 1)};
        REQUIRE(center(v) == PointF(0, 0));
        LineF x{PointF(-10, -11), PointF(-10, -9)};
        REQUIRE(center(x) == PointF(-10, -10));
    }
}

TEST_CASE("line length") {
    SECTION("horizontal") {
        LineF l{PointF(-1, 0), PointF(1, 0)};
        REQUIRE(l.length() == 2);
    }
    SECTION("diagonal") {
        LineF l{PointF(-1, -1), PointF(1, 1)};
        REQUIRE(l.length() == Approx(2.0 * std::sqrt(2)));
    }
}

TEST_CASE("line angle from origin") {
    SECTION("horizontal line to the right") {
        LineF l{PointF(0, 0), PointF(1, 0)};
        REQUIRE(l.angle() == 0);
    }
    SECTION("horizontal line to the left") {
        LineF l{PointF(0, 0), PointF(-1, 0)};
        REQUIRE(l.angle() == 180);
    }
    SECTION("vertical line to top") {
        LineF l{PointF(0, 0), PointF(0, -1)};
        REQUIRE(l.angle() == 90);
    }
    SECTION("vertical line to bottom") {
        LineF l{PointF(0, 0), PointF(0, 1)};
        REQUIRE(l.angle() == 270);
    }
    SECTION("horizontal line to bottom") {
        LineF l{PointF(0, 0), PointF(0, 1)};
        REQUIRE(l.angle() == 270);
    }
    SECTION("diagonal line to top right") {
        LineF l{PointF(0, 0), PointF(1, -1)};
        REQUIRE(l.angle() == 45);
    }
    SECTION("diagonal line to top left") {
        LineF l{PointF(0, 0), PointF(-1, -1)};
        REQUIRE(l.angle() == 135);
    }
}

TEST_CASE("line angle from (1,1)") {
    SECTION("horizontal line to the right") {
        LineF l{PointF(1, 1), PointF(2, 1)};
        REQUIRE(l.angle() == 0);
    }
    SECTION("horizontal line to the left") {
        LineF l{PointF(1, 1), PointF(0, 1)};
        REQUIRE(l.angle() == 180);
    }
    SECTION("vertical line to top") {
        LineF l{PointF(1, 1), PointF(1, 0)};
        REQUIRE(l.angle() == 90);
    }
    SECTION("vertical line to bottom") {
        LineF l{PointF(1, 1), PointF(1, 2)};
        REQUIRE(l.angle() == 270);
    }
    SECTION("horizontal line to bottom") {
        LineF l{PointF(1, 1), PointF(1, 2)};
        REQUIRE(l.angle() == 270);
    }
    SECTION("diagonal line to top right") {
        LineF l{PointF(1, 1), PointF(2, 0)};
        REQUIRE(l.angle() == 45);
    }
    SECTION("diagonal line to top left") {
        LineF l{PointF(1, 1), PointF(0, 0)};
        REQUIRE(l.angle() == 135);
    }
}
