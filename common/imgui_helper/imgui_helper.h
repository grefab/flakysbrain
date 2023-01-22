//
// Created by Gregor Fabritius on 20.10.22.
//

#pragma once

#include <imgui.h>
#include "common/geometry/PointF.h"
#include "common/proto/Point.pb.h"

ImVec2 operator+(ImVec2 const& a, ImVec2 const& b);
void operator+=(ImVec2& a, ImVec2 const& b);
ImVec2 operator-(ImVec2 const& a, ImVec2 const& b);
void operator-=(ImVec2& a, ImVec2 const& b);
ImVec2 operator*(ImVec2 const& a, float f);
void operator*=(ImVec2& a, float f);
ImVec2 operator/(ImVec2 const& a, float f);
void operator/=(ImVec2& a, float f);
float length(ImVec2 const& a);

ImVec2 asImVec2(common::Point const& point);
ImVec2 asImVec2(PointF const& point);
PointF asPointF(ImVec2 const& point);
common::Point asPoint(ImVec2 const& point);
