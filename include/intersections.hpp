#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>
#include <stdexcept>
#include <utility>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line &l1, const Line &l2) const {
        Point2D p1 = l1.start;
        Point2D p2 = l1.end;
        Point2D p3 = l2.start;
        Point2D p4 = l2.end;

        double d = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
        if (std::abs(d) < kEpsilon) {
            return std::nullopt;
        }

        double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / d;
        double u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / d;

        if (t < 0.0 || t > 1.0 || u < 0.0 || u > 1.0) {
            return std::nullopt;
        }

        return std::optional<Point2D>{std::in_place, p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)};
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const {
        Point2D d = line.end - line.start;
        Point2D f = line.start - circle.center_p;

        double a = d.Dot(d);
        double b = 2.0 * f.Dot(d);
        double c = f.Dot(f) - circle.radius * circle.radius;

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) {
            return std::nullopt;
        }

        discriminant = std::sqrt(discriminant);
        double t1 = (-b - discriminant) / (2.0 * a);
        double t2 = (-b + discriminant) / (2.0 * a);

        if (t1 >= 0.0 && t1 <= 1.0) {
            std::optional<Point2D> res;
            res.emplace(line.start + d * t1);
            return res;
        }
        if (t2 >= 0.0 && t2 <= 1.0) {
            std::optional<Point2D> res;
            res.emplace(line.start + d * t2);
            return res;
        }
        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Circle &circle, const Line &line) const { return (*this)(line, circle); }

    std::optional<Point2D> operator()(const Circle &c1, const Circle &c2) const {
        double d = c1.center_p.DistanceTo(c2.center_p);
        if (d > c1.radius + c2.radius || d < std::abs(c1.radius - c2.radius)) {
            return std::nullopt;
        }
        if (d < kEpsilon && std::abs(c1.radius - c2.radius) < kEpsilon) {
            return std::nullopt;
        }

        double a = (c1.radius * c1.radius - c2.radius * c2.radius + d * d) / (2.0 * d);
        double h_sq = c1.radius * c1.radius - a * a;
        if (h_sq < 0.0) {
            return std::nullopt;
        }
        double h = std::sqrt(h_sq);

        Point2D p2 = c1.center_p + (c2.center_p - c1.center_p) * (a / d);

        double rx = -(c2.center_p.y - c1.center_p.y) * (h / d);
        double ry = (c2.center_p.x - c1.center_p.x) * (h / d);

        return std::optional<Point2D>{std::in_place, p2.x + rx, p2.y + ry};
    }

    template <typename T, typename U>
    std::optional<Point2D> operator()(const T &, const U &) const {
        throw std::logic_error("Intersection not supported for these shape types");
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections