#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

// Ваш код здесь
void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    auto intersecting = others | views::filter([&shape](const Shape &other) {
                            return std::visit(
                                [](const auto &s1, const auto &s2) {
                                    using T1 = std::decay_t<decltype(s1)>;
                                    using T2 = std::decay_t<decltype(s2)>;
                                    return (std::is_same_v<T1, Line> && std::is_same_v<T2, Line>) ||
                                           (std::is_same_v<T1, Line> && std::is_same_v<T2, Circle>) ||
                                           (std::is_same_v<T1, Circle> && std::is_same_v<T2, Line>) ||
                                           (std::is_same_v<T1, Circle> && std::is_same_v<T2, Circle>);
                                },
                                shape, other);
                        });

    for (const auto &other : intersecting) {
        auto result = intersections::GetIntersectPoint(shape, other);
        if (result.has_value()) {
            std::println("Intersection found at point {} between shapes {} and {}", *result, shape, other);
        } else {
            std::println("Shapes {} and {} do not intersect", shape, other);
        }
    }
}

// Ваш код здесь
void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    auto selected = shapes | views::take(5);
    for (const auto &shape : selected) {
        double dist = queries::DistanceToPoint(shape, p);
        std::println("Distance from point {} to shape {} is {}", p, shape, dist);
    }
}

// Ваш код здесь
void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    auto collisions = utils::FindAllCollisions(shapes);
    std::println("Found {} collisions using Bounding Box method", collisions.size());
    for (const auto &[s1, s2] : collisions) {
        std::println("Collision between {} and {}", s1, s2);
    }

    auto highest = utils::FindHighestShape(shapes);
    if (highest.has_value()) {
        std::println("Highest shape is at index {} with height {}", *highest, queries::GetHeight(shapes[*highest]));
    }

    for (size_t i = 0; i < shapes.size(); ++i) {
        for (size_t j = i + 1; j < shapes.size(); ++j) {
            auto dist = queries::DistanceBetweenShapes(shapes[i], shapes[j]);
            if (dist.has_value()) {
                std::println("Distance between shapes at indices {} and {} is {}", i, j, *dist);
            }
        }
    }
}

// Ваш код здесь
void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    auto above_50 = shapes | views::filter([](const Shape &s) { return queries::GetHeight(s) > 50.0; });
    for (const auto &shape : above_50 | views::take(3)) {
        std::println("Shape above 50.0: {}", shape);
    }

    if (!shapes.empty()) {
        auto minmax = std::ranges::minmax_element(shapes, {}, [](const Shape &s) { return queries::GetHeight(s); });
        std::println("Shape with minimum height: {}", *minmax.min);
        std::println("Shape with maximum height: {}", *minmax.max);
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    for (size_t i = 0; i < shapes.size(); ++i) {
        std::println("Shape {} has height {}", i, queries::GetHeight(shapes[i]));
    }

    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    geometry::visualization::Draw(shapes);

    std::vector<Point2D> points;
    for (const auto &shape : shapes) {
        std::visit(
            [&points](const auto &s) {
                auto verts = s.Vertices();
                std::copy(verts.begin(), verts.end(), std::back_inserter(points));
            },
            shape);
    }

    // Ваш код здесь
    auto hull_result = convex_hull::GrahamScan(std::span{points});
    if (hull_result.has_value()) {
        auto hull_points = hull_result.value();
        std::println("Convex hull points:");
        for (const auto &p : hull_points) {
            std::println("\t{}", p);
        }
        shapes.push_back(Polygon{std::move(hull_points)});
        geometry::visualization::Draw(shapes);
    }

    // Ваш код здесь
    {
        std::vector<Point2D> tri_points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        auto tri_result = triangulation::DelaunayTriangulation(std::span{tri_points});
        if (tri_result.has_value()) {
            auto triangles = tri_result.value();
            std::println("Delaunay triangulation produced {} triangles", triangles.size());
            for (const auto &t : triangles) {
                std::println("{}", t);
            }
            geometry::visualization::Draw(triangles);
        }
    }
    return 0;
}