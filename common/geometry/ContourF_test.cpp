#include "common/geometry/ContourF.h"
#include <fmt/format.h>
#include "3rdparty/catch/catch.hpp"

TEST_CASE("centroid calculation") {
    // create approx circle using with r=4
    PointF offset{17, 23};
    ContourF circle = {
        offset + PointF{5, 0},
        offset + PointF{4, 3},
        offset + PointF{3, 4},
        offset + PointF{0, 5},
        offset + PointF{-3, 4},
        offset + PointF{-4, 3},
        offset + PointF{-5, 0},
        offset + PointF{-4, -3},
        offset + PointF{-3, -4},
        offset + PointF{0, -5},
        offset + PointF{3, -4},
        offset + PointF{4, -3},
    };

    REQUIRE(centroid(circle) == offset);
}

TEST_CASE("line polygon intersection") {
    // create approx circle using with r=4
    ContourF circle = {
        {5, 0},
        {4, 3},
        {3, 4},
        {0, 5},
        {-3, 4},
        {-4, 3},
        {-5, 0},
        {-4, -3},
        {-3, -4},
        {0, -5},
        {3, -4},
        {4, -3},
    };
    SECTION("intersection through origin") {
        LineF line{PointF(-10, 0), PointF(10, 0)};
        auto intersections = lineContourIntersections(line, circle);
        //        show_points(intersections);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0] == PointF(-5, 0));
        REQUIRE(intersections[1] == PointF(5, 0));
    }

    SECTION("intersection through somewhere") {
        LineF line{PointF(-5, -2), PointF(8, 1)};
        auto intersections = lineContourIntersections(line, circle);
        REQUIRE(intersections.size() == 2);
        //        show_points(intersections);
        REQUIRE(intersections[0].x == Approx(-4.38095));
        REQUIRE(intersections[0].y == Approx(-1.85714));
        REQUIRE(intersections[1].x == Approx(4.90476));
        REQUIRE(intersections[1].y == Approx(0.285714));
    }

    SECTION("intersection as secant") {
        LineF line{PointF(-5, -5), PointF(5, -5)};
        auto intersections = lineContourIntersections(line, circle);
        //        show_points(intersections);
        REQUIRE(intersections.size() == 1);
        REQUIRE(intersections[0] == PointF(0, -5));
    }

    SECTION("no intersection outside polygon") {
        LineF line{PointF(-5, -5), PointF(5, -6)};
        auto intersections = lineContourIntersections(line, circle);
        REQUIRE(intersections.empty());
    }
}

TEST_CASE("polygon polygon intersection") {
    ContourF rect = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1},
    };

    SECTION("intersection of rect and bigger triangle") {
        ContourF outer_triangle = {
            {2, -0.5},
            {0, 0.5},
            {2, 1.5},
        };
        auto intersected = contourIntersection(outer_triangle, rect);
        REQUIRE(intersected.size() == 1);
        auto result_triangle = intersected[0];
        ContourF inner_triangle = {
            {1, 0},
            {0, 0.5},
            {1, 1},
        };
        REQUIRE(result_triangle[0] == inner_triangle[2]);
        REQUIRE(result_triangle[1] == inner_triangle[1]);
        REQUIRE(result_triangle[2] == inner_triangle[0]);
    }

    SECTION("intersection of triangle with same border as outer rect") {
        ContourF inner_triangle = {
            {1, 0},
            {0, 0.5},
            {1, 1},
        };
        auto intersected = contourIntersection(inner_triangle, rect);
        REQUIRE(intersected.size() == 1);
        auto result_triangle = intersected[0];
        REQUIRE(result_triangle[0] == inner_triangle[2]);
        REQUIRE(result_triangle[1] == inner_triangle[1]);
        REQUIRE(result_triangle[2] == inner_triangle[0]);
    }

    SECTION("intersection of triangle within border") {
        ContourF inner_triangle = {
            {0.5, 0.25},
            {0, 0.5},
            {0.5, 0.75},
        };
        auto intersected = contourIntersection(inner_triangle, rect);
        REQUIRE(intersected.size() == 1);
        auto result_triangle = intersected[0];
        REQUIRE(result_triangle[0] == inner_triangle[2]);
        REQUIRE(result_triangle[1] == inner_triangle[1]);
        REQUIRE(result_triangle[2] == inner_triangle[0]);
    }
}
