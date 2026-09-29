#include "convex_hull.hpp"
#include <algorithm>

namespace geometry::convex_hull {

double CrossProduct(Point2D p1, Point2D middle, Point2D p2) {
    auto new_p1 = p1 - middle;
    auto new_p2 = p2 - middle;
    return new_p1.Cross(new_p2);
}

std::expected<std::vector<Point2D>, std::string> GrahamScan(std::span<Point2D> points) noexcept {
    if (points.size() < 3) {
        return std::unexpected<std::string>("At least three points are required for convex hull.");
    }

    // Найдена ошибка при помощи сохранения визуализации. Тесты все проходили. Выбиралась точка (1.5, 0), которая явно
    // лежала внутри! std::min_element использовал operator<, который не задавал строгие правила, а std::sort
    // начинал с points[0], который мог быть внутренней точкой. Так что без визуализации, которую добавила ИИшка, я бы
    // этот баг не нашел бы и не пофиксил
    //
    // Решил подписать тута =)
    auto min_it = std::ranges::min_element(points, {}, [](const Point2D &p) { return std::make_pair(p.y, p.x); });
    std::iter_swap(points.begin(), min_it);
    auto smallest = points.front();

    std::sort(points.begin() + 1, points.end(), [&smallest](const Point2D &p1, const Point2D &p2) {
        static const auto precision = 1e-10;

        double cross = CrossProduct(p1, smallest, p2);
        if (std::abs(cross) < precision) {
            return smallest.DistanceTo(p1) < smallest.DistanceTo(p2);
        }
        return cross > 0;
    });

    StackForGrahamScan hull;
    for (const auto &new_p : points) {
        while (hull.Size() > 1 && CrossProduct(hull.NextToTop(), hull.Top(), new_p) > 0.0) {
            hull.Pop();
        }
        hull.Push(new_p);
    }

    return std::vector{hull.Extract()};
}

}  // namespace geometry::convex_hull