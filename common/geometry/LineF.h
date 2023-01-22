//
// Created by user on 5/8/20.
//

#pragma once

#include <optional>
#include "common/geometry/PointF.h"

struct LineF {
    PointF a;
    PointF b;

    /// Returns the angle of the line in degrees.
    ///
    /// The return value will be in the range of values from 0.0
    /// up to but not including 360.0.
    /// The angles are measured counter-clockwise from a point
    /// on the x-axis to the right of the origin (x > 0).
    /// Note that origin is top left
    /// This is compatible to Qt's QLineF's angle() method.
    [[nodiscard]] float angle() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] PointF normalVector() const;

    // moves b so that |b-a| == length
    void setLength(float target_length);
};

std::optional<PointF> lineIntersection(LineF const& l1, LineF const& l2);
std::optional<PointF> lineSegmentIntersection(LineF const& l1, LineF const& l2);
PointF closestPointToLineSegment(LineF const& l, PointF const& p);
PointF projectedOnLine(LineF const& l, PointF const& p);
PointF center(LineF const& l);
float distance(LineF const& l, PointF const& p);
