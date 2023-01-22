//
// Created by user on 6/19/20.
//

#pragma once

#include "clipper2/clipper.h"
#include "common/geometry/ContourF.h"
#include "common/geometry/PointF.h"

Clipper2Lib::PathD asClipper2Path(ContourF const& contour);
ContourF asContourF(Clipper2Lib::PathD const& path);
Clipper2Lib::PathsD asClipper2Paths(ContoursF const& contours);
ContoursF asContoursF(Clipper2Lib::PathsD const& paths);

Clipper2Lib::PointD asClipper2Point(PointF const& p);
PointF asPointF(Clipper2Lib::PointD const& p);

bool isInPolygon(Clipper2Lib::PointD const& p, Clipper2Lib::PathD const& subject);
