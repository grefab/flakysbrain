#pragma once

#include "common/geometry/CircleF.h"
#include "common/geometry/ContourF.h"
#include "common/proto/Circle.pb.h"
#include "common/proto/Line.pb.h"
#include "common/proto/Path.pb.h"
#include "common/proto/Point.pb.h"
#include "common/proto/Rect.pb.h"

// conversion
PointF asPointF(common::Point const& p);
common::Point asPoint(PointF const& point);
LineF asLineF(common::Line const& line);
LineF asLineF(common::Point const& p1, common::Point const& p2);
common::Line asLine(LineF const& line);
RectF asRectF(common::Rect const& rect);
common::Rect asRect(RectF const& rect);
CircleF asCircleF(common::Circle const& circle);
common::Circle asCircle(CircleF const& circle);
ContourF asContourF(common::Path const& path);
common::Path asPath(ContourF const& contour);

// rectangle
LineF top(common::Rect const& r);
LineF bottom(common::Rect const& r);
LineF left(common::Rect const& r);
LineF right(common::Rect const& r);
PointF topLeft(common::Rect const& r);
PointF topRight(common::Rect const& r);
PointF bottomLeft(common::Rect const& r);
PointF bottomRight(common::Rect const& r);
