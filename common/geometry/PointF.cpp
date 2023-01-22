//
// Created by user on 5/8/20.
//

#include "common/geometry/PointF.h"

float distance(PointF const& a, PointF const& b) {
    return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
