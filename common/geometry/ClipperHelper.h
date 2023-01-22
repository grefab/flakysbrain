//
// Created by user on 6/19/20.
//

#pragma once

#include "3rdparty/clipper/clipper.hpp"
#include "common/geometry/ContourF.h"
#include "common/geometry/PointF.h"

ClipperLib::Path asClipperPath(ContourF const& contour);
ContourF asContourF(ClipperLib::Path const& path);

ClipperLib::IntPoint asClipperPoint(PointF const& p);
PointF asPoint(ClipperLib::IntPoint const& p);

bool isInPolygon(ClipperLib::IntPoint const& p, ClipperLib::Path const& subject);

ClipperLib::Paths intersectClosedPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip);
ClipperLib::Paths intersectOpenPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip);
ClipperLib::Paths subtractClosedPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip);
