#include "geometry.hpp"

namespace geometry {

std::vector<Point2D> RegularPolygon::Vertices() const {
    std::vector<Point2D> points;
    points.reserve(sides);

    for (int i = 0; i < sides; ++i) {
        const double angle = 2 * std::numbers::pi * i / sides;
        points.emplace_back(center_p.x + radius * std::cos(angle), center_p.y + radius * std::sin(angle));
    }
    return points;
}

Lines2DDyn RegularPolygon::Lines() const {
    auto verts = Vertices();
    Lines2DDyn lines;
    lines.Reserve(verts.size() + 1);
    for (const auto &p : verts) {
        lines.PushBack(p);
    }
    lines.PushBack(lines.Front());
    return lines;
}

std::vector<Point2D> Circle::Vertices(size_t N) const {
    std::vector<Point2D> points;
    points.reserve(N);

    for (auto i : std::ranges::views::iota(0u, N)) {
        const double angle = 2 * std::numbers::pi * i / N;
        points.emplace_back(center_p.x + radius * std::cos(angle), center_p.y + radius * std::sin(angle));
    }
    return points;
}

Lines2DDyn Circle::Lines(size_t N) const {
    Lines2DDyn lines;
    lines.Reserve(N + 1);
    for (auto i : std::ranges::views::iota(0u, N)) {
        double angle = 2 * std::numbers::pi * i / N;
        lines.PushBack(center_p.x + radius * std::cos(angle), center_p.y + radius * std::sin(angle));
    }
    lines.PushBack(lines.Front());
    return lines;
}

void Polygon::CalculateBoundBox() {
    double min_x = points_[0].x, max_x = points_[0].x;
    double min_y = points_[0].y, max_y = points_[0].y;

    for (const auto &p : points_) {
        if (p.x < min_x)
            min_x = p.x;
        if (p.x > max_x)
            max_x = p.x;
        if (p.y < min_y)
            min_y = p.y;
        if (p.y > max_y)
            max_y = p.y;
    }

    bounding_box_ = BoundingBox{min_x, min_y, max_x, max_y};
}

}  // namespace geometry