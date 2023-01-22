//
// Created by user on 5/8/20.
//

#pragma once

#include <opencv2/core/core.hpp>
#include <optional>
#include "common/geometry/LineF.h"

// using RectF = cv::Rect2f;
class RectF : public cv::Rect2f {
public:
    RectF() : Rect_(){};
    RectF(float x, float y, float width, float height) : Rect_(x, y, width, height) {}
    RectF(cv::Point_<float> const& p1, cv::Point_<float> const& p2) : Rect_(p1, p2) {}
    RectF(cv::Point_<float> const& p1, cv::Size_<float> const& s) : Rect_(p1, s) {}
    RectF(RectF const& r) : Rect_(r) {}
    RectF(RectF&& r) : Rect_(r) {}
    RectF(PointF const& org, float width, float height) : RectF(org.x, org.y, width, height) {}
};

cv::Rect asCvRect(RectF const& rect);

// selection
LineF top(RectF const& rect);
LineF bottom(RectF const& rect);
LineF left(RectF const& rect);
LineF right(RectF const& rect);
PointF topLeft(RectF const& rect);
PointF topRight(RectF const& rect);
PointF bottomLeft(RectF const& rect);
PointF bottomRight(RectF const& rect);
RectF leftPart(RectF const& rect, float ratio);
RectF leftHalf(RectF const& rect);
RectF rightPart(RectF const& rect, float ratio);
RectF rightHalf(RectF const& rect);

RectF translated(RectF const& rect, PointF const& translation_vector);

std::optional<LineF> clipLineInRect(LineF const& line, RectF const& clipRect);
