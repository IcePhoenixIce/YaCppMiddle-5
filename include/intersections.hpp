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
    std::optional<Point2D> operator()(const Line &l1, const Line &l2) const;
    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const;
    std::optional<Point2D> operator()(const Circle &circle, const Line &line) const;
    std::optional<Point2D> operator()(const Circle &c1, const Circle &c2) const;

    template <typename T, typename U>
    std::optional<Point2D> operator()(const T &, const U &) const {
        throw std::logic_error("Intersection not supported for these shape types");
    }
};

std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2);

}  // namespace geometry::intersections