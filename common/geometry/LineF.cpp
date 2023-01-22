//
// Created by user on 5/8/20.
//

#include "common/geometry/LineF.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

std::optional<PointF> lineIntersection(LineF const& l1, LineF const& l2) {
    // see: https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/

    // Line AB represented as a1x + b1y = c1
    double a1 = l1.b.y - l1.a.y;
    double b1 = l1.a.x - l1.b.x;
    double c1 = a1 * (l1.a.x) + b1 * (l1.a.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = l2.b.y - l2.a.y;
    double b2 = l2.a.x - l2.b.x;
    double c2 = a2 * (l2.a.x) + b2 * (l2.a.y);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        // lines are parallel
        return std::nullopt;
    } else {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return PointF(x, y);
    }
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
inline bool onSegment(PointF const& p, PointF const& q, PointF const& r) {
    // see: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y)
            && q.y >= std::min(p.y, r.y));
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
inline int orientation(PointF const& p, PointF const& q, PointF const& r) {
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0) {
        return 0;  // colinear
    }

    return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

// The main function that returns true if line segment l1
// and l2 intersect.
inline bool doIntersect(LineF const& l1, LineF const& l2) {
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(l1.a, l1.b, l2.a);
    int o2 = orientation(l1.a, l1.b, l2.b);
    int o3 = orientation(l2.a, l2.b, l1.a);
    int o4 = orientation(l2.a, l2.b, l1.b);

    // General case
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(l1.a, l2.a, l1.b)) {
        return true;
    }

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(l1.a, l2.b, l1.b)) {
        return true;
    }

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(l2.a, l1.a, l2.b)) {
        return true;
    }

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(l2.a, l1.b, l2.b)) {
        return true;
    }

    return false;  // Doesn't fall in any of the above cases
}

std::optional<PointF> lineSegmentIntersection(LineF const& l1, LineF const& l2) {
    if (!doIntersect(l1, l2)) {
        return std::nullopt;
    }
    return lineIntersection(l1, l2);
}

float LineF::angle() const {
    float const dx = b.x - a.x;
    float const dy = b.y - a.y;
    float const theta = std::atan2(-dy, dx) * 360.0 / (2.0 * M_PI);
    float const theta_normalized = theta < 0 ? theta + 360.0 : theta;
    if (theta_normalized >= 360) {
        return 0;
    }
    return theta_normalized;
}

float LineF::length() const {
    return distance(a, b);
}

PointF LineF::normalVector() const {
    return (b - a) / length();
}

void LineF::setLength(float target_length) {
    b = a + normalVector() * target_length;
}

PointF closestPointToLineSegment(LineF const& l, PointF const& p) {
    PointF const& a = l.a;
    PointF const& b = l.b;
    PointF const ap = p - a;
    PointF const ab_dir = b - a;
    float const dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    if (dot < 0.0f)
        return a;
    float const ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
    if (dot > ab_len_sqr)
        return b;
    return a + ab_dir * dot / ab_len_sqr;
}

PointF projectedOnLine(LineF const& l, PointF const& p) {
    PointF const& a = l.a;
    PointF const& b = l.b;
    PointF const ap = p - a;
    PointF const ab_dir = b - a;
    float const dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    float const ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
    return a + ab_dir * dot / ab_len_sqr;
}

PointF center(LineF const& l) {
    return (l.a + l.b) / 2.0f;
}

float distance(LineF const& l, PointF const& p) {
    return distance(p, projectedOnLine(l, p));
}
