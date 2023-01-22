//
// Created by user on 5/8/20.
//

#pragma once

#include <functional>
#include <opencv2/core/core.hpp>
#include <vector>
#include "common/geometry/LineF.h"
#include "common/geometry/PointF.h"
#include "common/geometry/RectF.h"

using ContourF = std::vector<PointF>;
using ContoursF = std::vector<ContourF>;

RectF boundingBox(ContourF const& contour);
PointF centroid(ContourF const& contour);
PointF closestTo(std::vector<PointF> const& points, PointF const& p);
std::vector<PointF> lineContourIntersections(LineF const& l, ContourF const& p);
ContoursF contourIntersection(ContourF const& a, ContourF const& b);
ContoursF contourSubtraction(ContourF const& a, ContourF const& b);
ContoursF clipContourInRect(ContourF const& contour, RectF const& clipRect);

ContourF translated(ContourF const& contour, PointF const& t);
ContourF convexHull(ContourF const& contour);
LineF getMaxDistanceToConvex(ContourF const& contour, ContourF* convex_contour_out = nullptr);

ContourF asContourF(std::vector<cv::Point> const& contour);
ContourF asContourF(LineF const& line);
ContourF asContourF(RectF const& rect);

bool isInPolygon(PointF const& p, ContourF const& c);
