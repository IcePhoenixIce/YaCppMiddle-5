#pragma once
#include "geometry.hpp"
#include <algorithm>
#include <expected>
#include <format>
#include <set>
#include <vector>

namespace geometry::triangulation {

struct DelaunayTriangle {
    Point2D a, b, c;

    DelaunayTriangle(Point2D a, Point2D b, Point2D c) : a(a), b(b), c(c) {}

    bool ContainsPoint(const Point2D &p) const;
    Point2D Circumcenter() const;
    double Circumradius() const;
    bool SharesEdge(const DelaunayTriangle &other) const;
    std::vector<Point2D> vertices() const;
};

struct Edge {
    Point2D p1, p2;

    Edge(Point2D p1, Point2D p2) : p1(p1), p2(p2) {
        if (p1.x > p2.x || (p1.x == p2.x && p1.y > p2.y)) {
            std::swap(this->p1, this->p2);
        }
    }

    bool operator<(const Edge &other) const {
        if (std::abs(p1.x - other.p1.x) > kEpsilon)
            return p1.x < other.p1.x;
        if (std::abs(p1.y - other.p1.y) > kEpsilon)
            return p1.y < other.p1.y;
        if (std::abs(p2.x - other.p2.x) > kEpsilon)
            return p2.x < other.p2.x;
        return p2.y < other.p2.y;
    }

    bool operator==(const Edge &other) const {
        return std::abs(p1.x - other.p1.x) < kEpsilon && std::abs(p1.y - other.p1.y) < kEpsilon &&
               std::abs(p2.x - other.p2.x) < kEpsilon && std::abs(p2.y - other.p2.y) < kEpsilon;
    }
};

std::expected<std::vector<DelaunayTriangle>, std::string>
DelaunayTriangulation(std::span<const Point2D> points) noexcept;

}  // namespace geometry::triangulation

template <>
struct std::formatter<geometry::triangulation::DelaunayTriangle> {
    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const geometry::triangulation::DelaunayTriangle &t, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "DelaunayTriangle({}, {}, {})", t.a, t.b, t.c);
    }
};