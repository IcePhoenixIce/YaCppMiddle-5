#pragma once
#include "geometry.hpp"
#include <algorithm>
#include <limits>
#include <optional>
#include <variant>

namespace geometry::queries {

template <class... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};

struct DistanceVisitor {
    Point2D point;

    explicit DistanceVisitor(const Point2D &p);
    double operator()(const Line &line) const;
    double operator()(const Triangle &triangle) const;
    double operator()(const Rectangle &rect) const;
    double operator()(const RegularPolygon &polygon) const;
    double operator()(const Circle &circle) const;
    double operator()(const Polygon &polygon) const;
};

struct PointToShapeDistanceVisitor {
    Point2D point;

    explicit PointToShapeDistanceVisitor(const Point2D &p);
    double operator()(const Line &line) const;
    double operator()(const Triangle &triangle) const;
    double operator()(const Rectangle &rect) const;
    double operator()(const RegularPolygon &polygon) const;
    double operator()(const Circle &circle) const;
    double operator()(const Polygon &polygon) const;
};

struct PointInShapeVisitor {
    Point2D point;

    explicit PointInShapeVisitor(const Point2D &p);
    bool operator()(const Line &line) const;
    bool operator()(const Triangle &triangle) const;
    bool operator()(const Rectangle &rect) const;
    bool operator()(const RegularPolygon &polygon) const;
    bool operator()(const Circle &circle) const;

private:
    bool point_in_polygon_ray_casting(const Point2D &p, const std::vector<Point2D> &vertices) const;
};

struct ShapeToShapeDistanceVisitor {
    std::optional<double> operator()(const Circle &c1, const Circle &c2) const;
    std::optional<double> operator()(const Line &l1, const Line &l2) const;

    template <typename T, typename U>
    std::optional<double> operator()(const T &, const U &) const {
        return std::nullopt;
    }
};

double DistanceToPoint(const Shape &shape, const Point2D &point);
BoundingBox GetBoundBox(const Shape &shape);
double GetHeight(const Shape &shape);
bool BoundingBoxesOverlap(const Shape &shape1, const Shape &shape2);
std::optional<double> DistanceBetweenShapes(const Shape &shape1, const Shape &shape2);

}  // namespace geometry::queries