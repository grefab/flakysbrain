//
// Created by user on 6/19/20.
//

#include "common/geometry/Clipper2Helper.h"
#include <range/v3/all.hpp>

Clipper2Lib::PathD asClipper2Path(ContourF const& contour) {
    return contour | ranges::views::transform([](auto const& point) { return asClipper2Point(point); })
           | ranges::to_vector;
}

ContourF asContourF(Clipper2Lib::PathD const& path) {
    return path | ranges::views::transform([](auto const& point) { return asPointF(point); }) | ranges::to_vector;
}

Clipper2Lib::PathsD asClipper2Paths(ContoursF const& contours) {
    return contours | ranges::views::transform([](auto const& contour) { return asClipper2Path(contour); })
           | ranges::to_vector;
}

ContoursF asContoursF(Clipper2Lib::PathsD const& paths) {
    return paths | ranges::views::transform([](auto const& path) { return asContourF(path); }) | ranges::to_vector;
}

Clipper2Lib::PointD asClipper2Point(PointF const& p) {
    return {p.x, p.y};
}

PointF asPointF(Clipper2Lib::PointD const& p) {
    return PointF(p.x, p.y);
}

bool isInPolygon(Clipper2Lib::PointD const& p, Clipper2Lib::PathD const& subject) {
    return false;
}
