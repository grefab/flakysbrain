//
// Created by user on 5/8/20.
//

#include "common/geometry/RectF.h"
#include <range/v3/all.hpp>
#include "common/geometry/ClipperHelper.h"
#include "common/geometry/ContourF.h"

cv::Rect asCvRect(RectF const& rect) {
    return cv::Rect(rect.x, rect.y, rect.width, rect.height);
}

LineF top(RectF const& rect) {
    return LineF{PointF(rect.x, rect.y), PointF(rect.x + rect.width, rect.y)};
}

LineF bottom(RectF const& rect) {
    return LineF{PointF(rect.x, rect.y + rect.height), PointF(rect.x + rect.width, rect.y + rect.height)};
}

LineF left(RectF const& rect) {
    return LineF{PointF(rect.x, rect.y), PointF(rect.x, rect.y + rect.height)};
}

LineF right(RectF const& rect) {
    return LineF{PointF(rect.x + rect.width, rect.y), PointF(rect.x + rect.width, rect.y + rect.height)};
}

PointF topLeft(RectF const& rect) {
    return {rect.x, rect.y};
}

PointF topRight(RectF const& rect) {
    return {rect.x + rect.width, rect.y};
}

PointF bottomLeft(RectF const& rect) {
    return {rect.x, rect.y + rect.height};
}

PointF bottomRight(RectF const& rect) {
    return {rect.x + rect.width, rect.y + rect.height};
}

// ratio = 1: complete rectangle
// ratio = 0: nothing
// ratio = 0.5: equals leftHalf()
RectF leftPart(RectF const& rect, float ratio) {
    auto top_left = rect.tl();
    auto bottom_right = rect.br();
    bottom_right.x = top_left.x + (bottom_right.x - top_left.x) * ratio;
    return {top_left, bottom_right};
}

RectF leftHalf(RectF const& rect) {
    auto top_left = rect.tl();
    auto bottom_right = rect.br();
    bottom_right.x = (top_left.x + bottom_right.x) / 2.0;
    return {top_left, bottom_right};
}

// ratio = 1: complete rectangle
// ratio = 0: nothing
// ratio = 0.5: equals rightHalf()
RectF rightPart(RectF const& rect, float ratio) {
    auto top_left = rect.tl();
    auto bottom_right = rect.br();
    top_left.x = bottom_right.x - (bottom_right.x - top_left.x) * ratio;
    return {top_left, bottom_right};
}

RectF rightHalf(RectF const& rect) {
    auto top_left = rect.tl();
    auto bottom_right = rect.br();
    top_left.x = (top_left.x + bottom_right.x) / 2.0;
    return {top_left, bottom_right};
}

RectF translated(RectF const& rect, PointF const& translation_vector) {
    return {rect.x + translation_vector.x, rect.y + translation_vector.y, rect.width, rect.height};
}

std::optional<LineF> clipLineInRect(LineF const& line, RectF const& clipRect) {
    auto contour = asContourF(line);
    auto solutions = intersectOpenPath(asClipperPath(contour), asClipperPath(asContourF(clipRect)));
    auto contours = solutions | ranges::views::transform([](auto const& contour) { return asContourF(contour); })
                    | ranges::to_vector;
    if (contours.size() != 1) {
        return std::nullopt;
    }
    return LineF{.a = contours[0][0], .b = contours[0][1]};
}
