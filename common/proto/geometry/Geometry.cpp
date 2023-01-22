#include "common/proto/geometry/Geometry.h"
#include <range/v3/all.hpp>

PointF asPointF(common::Point const& p) {
    return PointF(p.x(), p.y());
}

common::Point asPoint(PointF const& point) {
    common::Point p;
    p.set_x(point.x);
    p.set_y(point.y);
    return p;
}

LineF asLineF(common::Line const& line) {
    return LineF{asPointF(line.p1()), asPointF(line.p2())};
}

LineF asLineF(common::Point const& p1, common::Point const& p2) {
    return LineF{asPointF(p1), asPointF(p2)};
}

common::Line asLine(LineF const& line) {
    common::Line l;
    *l.mutable_p1() = asPoint(line.a);
    *l.mutable_p2() = asPoint(line.b);
    return l;
}

RectF asRectF(common::Rect const& rect) {
    return RectF(rect.x(), rect.y(), rect.w(), rect.h());
}

common::Rect asRect(RectF const& rect) {
    common::Rect r;
    r.set_x(rect.x);
    r.set_y(rect.y);
    r.set_w(rect.width);
    r.set_h(rect.height);
    return r;
}

CircleF asCircleF(common::Circle const& circle) {
    CircleF c;
    c.center = asPointF(circle.center());
    c.radius = circle.radius();
    return c;
}
common::Circle asCircle(CircleF const& circle) {
    common::Circle c;
    *c.mutable_center() = asPoint(circle.center);
    c.set_radius(circle.radius);
    return c;
}

ContourF asContourF(common::Path const& path) {
    return path.points() | ranges::views::transform([](auto const& p) { return cv::Point2f(p.x(), p.y()); })
           | ranges::to_vector;
}

common::Path asPath(ContourF const& contour) {
    common::Path result;
    for (auto const& p : contour) {
        *result.add_points() = asPoint(p);
    }
    return result;
}

LineF top(common::Rect const& r) {
    return LineF{PointF(r.x(), r.y()), PointF(r.x() + r.w(), r.y())};
}

LineF bottom(common::Rect const& r) {
    return LineF{PointF(r.x(), r.y() + r.h()), PointF(r.x() + r.w(), r.y() + r.h())};
}

LineF left(common::Rect const& r) {
    return LineF{PointF(r.x(), r.y()), PointF(r.x(), r.y() + r.h())};
}

LineF right(common::Rect const& r) {
    return LineF{PointF(r.x() + r.w(), r.y()), PointF(r.x() + r.w(), r.y() + r.h())};
}

PointF topLeft(common::Rect const& r) {
    return PointF(r.x(), r.y());
}

PointF topRight(common::Rect const& r) {
    return PointF(r.x() + r.w(), r.y());
}

PointF bottomLeft(common::Rect const& r) {
    return PointF(r.x(), r.y() + r.h());
}

PointF bottomRight(common::Rect const& r) {
    return PointF(r.x() + r.w(), r.y() + r.h());
}
