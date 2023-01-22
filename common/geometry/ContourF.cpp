//
// Created by user on 5/8/20.
//

#include "common/geometry/ContourF.h"
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <range/v3/all.hpp>
#include "common/geometry/ClipperHelper.h"

RectF boundingBox(ContourF const& contour) {
    PointF min(100000, 100000);
    PointF max(-100000, -100000);
    for (auto const& p : contour) {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
    }
    return RectF{min, max};
}

PointF centroid(ContourF const& contour) {
    PointF sum{0, 0};
    for (auto const& p : contour) {
        sum.x += p.x;
        sum.y += p.y;
    }
    return PointF{sum.x / (float)contour.size(), sum.y / (float)contour.size()};
}

PointF closestTo(std::vector<PointF> const& points, PointF const& p) {
    return *std::min_element(std::begin(points), std::end(points), [&](auto const& a, auto const& b) {
        return distance(a, p) < distance(b, p);
    });
}

std::vector<PointF> lineContourIntersections(LineF const& l, ContourF const& p) {
    std::vector<PointF> result;
    // we have no intersection with empty things
    if (p.empty()) {
        return result;
    }

    auto add_to_result_if_valid = [&](auto intersection) {
        if (intersection) {
            // clean up numerical particularities
            if (intersection->x == -0.0) {
                intersection->x = 0.0;
            }
            if (intersection->y == -0.0) {
                intersection->y = 0.0;
            }
            result.emplace_back(*intersection);
        }
    };

    // go through every line segment of the polygon
    for (size_t i = 0; i < p.size() - 1; ++i) {
        LineF segment{p[i], p[i + 1]};
        add_to_result_if_valid(lineSegmentIntersection(l, segment));
    }

    // process the line that connects the last and the first point,
    // i.e. treat polygon as closed
    if (p.front() != p.back()) {
        LineF segment{p.back(), p.front()};
        add_to_result_if_valid(lineSegmentIntersection(l, segment));
    }

    // sort all intersection points along line by distance to origin of line
    // and remove all duplicate entries (they happen when crossing exactly
    // through a node; then two line segments are hit)
    result |= ranges::actions::sort([&](auto const& a, auto const& b) { return distance(l.a, a) < distance(l.a, b); })
              | ranges::actions::unique;
    return result;
}

ContoursF contourIntersection(ContourF const& a, ContourF const& b) {
    auto solutions = intersectClosedPath(asClipperPath(a), asClipperPath(b));
    return solutions | ranges::views::transform([](auto const& contour) { return asContourF(contour); })
           | ranges::to_vector;
}

ContoursF contourSubtraction(ContourF const& a, ContourF const& b) {
    auto solutions = subtractClosedPath(asClipperPath(a), asClipperPath(b));
    return solutions | ranges::views::transform([](auto const& contour) { return asContourF(contour); })
           | ranges::to_vector;
}

ContoursF clipContourInRect(ContourF const& contour, RectF const& clipRect) {
    auto solutions = intersectOpenPath(asClipperPath(contour), asClipperPath(asContourF(clipRect)));
    return solutions | ranges::views::transform([](auto const& contour) { return asContourF(contour); })
           | ranges::to_vector;
}

ContourF translated(ContourF const& contour, PointF const& t) {
    ContourF result;
    result.reserve(contour.size());
    std::transform(std::begin(contour), std::end(contour), std::back_inserter(result), [&t](auto const& e) {
        return e + t;
    });
    return result;
}

ContourF convexHull(ContourF const& contour) {
    ContourF hull;
    cv::convexHull(contour, hull);
    return hull;
}

LineF getMaxDistanceToConvex(ContourF const& contour, ContourF* convex_contour_out) {
    LineF max_distance_line;

    ContourF convex_contour = convexHull(contour);
    std::vector<LineF> convex_lines(0);
    for (int i = 0; i < convex_contour.size() - 1; ++i) {
        convex_lines.emplace_back(LineF{
            .a = convex_contour[i],
            .b = convex_contour[i + 1],
        });
    }
    convex_lines.emplace_back(LineF{
        .a = convex_contour[convex_contour.size() - 1],
        .b = convex_contour[0],
    });

    struct DistancePair {
        PointF p_contour;
        PointF p_on_convex_contour;
        float distance;
    };
    std::vector<DistancePair> farthest_points_on_convex_contour;
    for (auto const& p : contour) {
        std::vector<DistancePair> closest_points_for_all_lines;
        for (auto const& l : convex_lines) {
            auto p_closest = closestPointToLineSegment(l, p);
            closest_points_for_all_lines.emplace_back(DistancePair{
                .p_contour = p,
                .p_on_convex_contour = p_closest,
                .distance = distance(p, p_closest),
            });
        }

        DistancePair closest_distance_pair =
            *std::min_element(std::begin(closest_points_for_all_lines),
                              std::end(closest_points_for_all_lines),
                              [&](auto const& a, auto const& b) { return a.distance < b.distance; });
        farthest_points_on_convex_contour.push_back(closest_distance_pair);
    }
    auto farthest_point_on_convex_contour =
        *std::max_element(std::begin(farthest_points_on_convex_contour),
                          std::end(farthest_points_on_convex_contour),
                          [&](auto const& a, auto const& b) { return a.distance < b.distance; });
    max_distance_line.a = farthest_point_on_convex_contour.p_contour;
    max_distance_line.b = farthest_point_on_convex_contour.p_on_convex_contour;

    if (convex_contour_out != nullptr) {
        *convex_contour_out = convex_contour;
    }
    return max_distance_line;
}

ContourF asContourF(std::vector<cv::Point> const& contour) {
    ContourF result = contour | ranges::views::transform([](auto const& point) {
                          // pixel center compensation
                          return PointF(point.x + 0.5, point.y + 0.5);
                      })
                      | ranges::to_vector;
    return result;
}

ContourF asContourF(LineF const& line) {
    ContourF result;
    result.emplace_back(line.a.x, line.a.y);
    result.emplace_back(line.b.x, line.b.y);
    return result;
}

ContourF asContourF(RectF const& rect) {
    ContourF result;
    result.emplace_back(rect.x, rect.y);
    result.emplace_back(rect.x + rect.width, rect.y);
    result.emplace_back(rect.x + rect.width, rect.y + rect.height);
    result.emplace_back(rect.x, rect.y + rect.height);
    return result;
}

bool isInPolygon(PointF const& p, ContourF const& c) {
    return isInPolygon(asClipperPoint(p), asClipperPath(c));
}
