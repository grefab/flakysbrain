//
// Created by user on 6/19/20.
//

#include "common/geometry/ClipperHelper.h"

ClipperLib::Path asClipperPath(ContourF const& contour) {
    ClipperLib::Path result;
    std::transform(std::begin(contour), std::end(contour), std::back_inserter(result), [](auto const& e) {
        return ClipperLib::IntPoint(e.x * 1000000.0, e.y * 1000000.0);
    });
    return result;
}

ContourF asContourF(ClipperLib::Path const& path) {
    ContourF result;
    std::transform(std::begin(path), std::end(path), std::back_inserter(result), [](auto const& e) {
        return cv::Point2f(e.X / 1000000.0, e.Y / 1000000.0);
    });
    return result;
}

ClipperLib::IntPoint asClipperPoint(PointF const& p) {
    return ClipperLib::IntPoint(p.x * 1000000.0, p.y * 1000000.0);
}

PointF asPoint(ClipperLib::IntPoint const& p) {
    return PointF(p.X / 1000000.0, p.Y / 1000000.0);
}

bool isInPolygon(ClipperLib::IntPoint const& p, ClipperLib::Path const& subject) {
    return ClipperLib::PointInPolygon(p, subject);
}

ClipperLib::Paths intersectClosedPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip) {
    ClipperLib::Paths paths;
    ClipperLib::Clipper c;
    c.AddPath(subject, ClipperLib::PolyType::ptSubject, true);
    c.AddPath(clip, ClipperLib::PolyType::ptClip, true);
    c.Execute(ClipperLib::ClipType::ctIntersection,
              paths,
              ClipperLib::PolyFillType::pftEvenOdd,
              ClipperLib::PolyFillType::pftEvenOdd);
    return paths;
}

ClipperLib::Paths intersectOpenPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip) {
    ClipperLib::PolyTree poly_tree;
    ClipperLib::Clipper c;
    c.AddPath(subject, ClipperLib::PolyType::ptSubject, false);
    c.AddPath(clip, ClipperLib::PolyType::ptClip, true);
    c.Execute(ClipperLib::ClipType::ctIntersection,
              poly_tree,
              ClipperLib::PolyFillType::pftEvenOdd,
              ClipperLib::PolyFillType::pftEvenOdd);

    ClipperLib::Paths paths;
    ClipperLib::OpenPathsFromPolyTree(poly_tree, paths);
    return paths;
}

ClipperLib::Paths subtractClosedPath(ClipperLib::Path const& subject, ClipperLib::Path const& clip) {
    ClipperLib::Paths solutions;
    ClipperLib::Clipper c;
    c.AddPath(subject, ClipperLib::PolyType::ptSubject, true);
    c.AddPath(clip, ClipperLib::PolyType::ptClip, true);
    c.Execute(ClipperLib::ClipType::ctDifference,
              solutions,
              ClipperLib::PolyFillType::pftEvenOdd,
              ClipperLib::PolyFillType::pftEvenOdd);
    return solutions;
}
