//
// Created by Gregor Fabritius on 21.10.22.
//

#include "common/imgui_helper/imgui_helper.h"

ImVec2 operator+(ImVec2 const& a, ImVec2 const& b) {
    return {a.x + b.x, a.y + b.y};
}

void operator+=(ImVec2& a, ImVec2 const& b) {
    a.x += b.x;
    a.y += b.y;
}

ImVec2 operator-(ImVec2 const& a, ImVec2 const& b) {
    return {a.x - b.x, a.y - b.y};
}

void operator-=(ImVec2& a, ImVec2 const& b) {
    a.x -= b.x;
    a.y -= b.y;
}

ImVec2 operator*(ImVec2 const& a, float f) {
    return {a.x * f, a.y * f};
}

void operator*=(ImVec2& a, float f) {
    a = a * f;
}

ImVec2 operator/(ImVec2 const& a, float f) {
    return {a.x / f, a.y / f};
}

void operator/=(ImVec2& a, float f) {
    a = a / f;
}

float length(ImVec2 const& a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}

ImVec2 asImVec2(common::Point const& point) {
    return {static_cast<float>(point.x()), static_cast<float>(point.y())};
}

ImVec2 asImVec2(PointF const& point) {
    return {point.x, point.y};
}

PointF asPointF(ImVec2 const& point) {
    return PointF{point.x, point.y};
}

common::Point asPoint(ImVec2 const& point) {
    common::Point p;
    p.set_x(point.x);
    p.set_y(point.y);
    return p;
}
