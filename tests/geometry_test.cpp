#include "geometry.hpp"
#include <cmath>
#include <gtest/gtest.h>

using namespace geometry;

TEST(GeometryTest, Point2D_Operators) {
    Point2D a{3, 4};
    Point2D b{1, 2};

    Point2D sum = a + b;
    EXPECT_DOUBLE_EQ(sum.x, 4);
    EXPECT_DOUBLE_EQ(sum.y, 6);

    Point2D diff = a - b;
    EXPECT_DOUBLE_EQ(diff.x, 2);
    EXPECT_DOUBLE_EQ(diff.y, 2);

    Point2D mul = a * 2;
    EXPECT_DOUBLE_EQ(mul.x, 6);
    EXPECT_DOUBLE_EQ(mul.y, 8);

    Point2D div = a / 2;
    EXPECT_DOUBLE_EQ(div.x, 1.5);
    EXPECT_DOUBLE_EQ(div.y, 2);
}

TEST(GeometryTest, Point2D_DotCross) {
    Point2D a{3, 4};
    Point2D b{1, 2};

    EXPECT_DOUBLE_EQ(a.Dot(b), 3 * 1 + 4 * 2);
    EXPECT_DOUBLE_EQ(a.Cross(b), 3 * 2 - 4 * 1);
}

TEST(GeometryTest, Point2D_Length) {
    Point2D a{3, 4};
    EXPECT_DOUBLE_EQ(a.Length(), 5);
}

TEST(GeometryTest, Point2D_DistanceTo) {
    Point2D a{0, 0};
    Point2D b{3, 4};
    EXPECT_DOUBLE_EQ(a.DistanceTo(b), 5);
}

TEST(GeometryTest, Point2D_Normalize) {
    Point2D a{3, 4};
    Point2D n = a.Normalize();
    EXPECT_DOUBLE_EQ(n.Length(), 1.0);
    EXPECT_DOUBLE_EQ(n.x, 0.6);
    EXPECT_DOUBLE_EQ(n.y, 0.8);
}

TEST(GeometryTest, Point2D_Less) {
    Point2D a{1, 2};
    Point2D b{2, 1};
    Point2D c{1, 3};

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a < c);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a < a);
}

TEST(GeometryTest, BoundingBox_Overlaps) {
    BoundingBox a{0, 0, 2, 2};
    BoundingBox b{1, 1, 3, 3};
    BoundingBox c{3, 3, 5, 5};

    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_TRUE(b.Overlaps(a));
    EXPECT_FALSE(a.Overlaps(c));
}

TEST(GeometryTest, BoundingBox_Dimensions) {
    BoundingBox box{1, 2, 5, 8};
    EXPECT_DOUBLE_EQ(box.Width(), 4);
    EXPECT_DOUBLE_EQ(box.Height(), 6);
    EXPECT_DOUBLE_EQ(box.Center().x, 3);
    EXPECT_DOUBLE_EQ(box.Center().y, 5);
}

TEST(GeometryTest, Line_Properties) {
    Line line{{0, 0}, {3, 4}};
    EXPECT_DOUBLE_EQ(line.Length(), 5);
    EXPECT_DOUBLE_EQ(line.Height(), 4);

    auto dir = line.Direction();
    EXPECT_DOUBLE_EQ(dir.x, 0.6);
    EXPECT_DOUBLE_EQ(dir.y, 0.8);

    auto center = line.Center();
    EXPECT_DOUBLE_EQ(center.x, 1.5);
    EXPECT_DOUBLE_EQ(center.y, 2);

    auto box = line.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0);
    EXPECT_DOUBLE_EQ(box.max_x, 3);
    EXPECT_DOUBLE_EQ(box.min_y, 0);
    EXPECT_DOUBLE_EQ(box.max_y, 4);

    auto verts = line.Vertices();
    EXPECT_EQ(verts.size(), 2);
    EXPECT_EQ(verts[0].x, 0);
    EXPECT_EQ(verts[1].x, 3);
}

TEST(GeometryTest, Triangle_Properties) {
    Triangle tri{{0, 0}, {4, 0}, {0, 3}};
    EXPECT_DOUBLE_EQ(tri.Area(), 6);
    EXPECT_DOUBLE_EQ(tri.Height(), 3);

    auto center = tri.Center();
    EXPECT_DOUBLE_EQ(center.x, 4.0 / 3.0);
    EXPECT_DOUBLE_EQ(center.y, 1.0);

    auto box = tri.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 0);
    EXPECT_DOUBLE_EQ(box.max_x, 4);
    EXPECT_DOUBLE_EQ(box.min_y, 0);
    EXPECT_DOUBLE_EQ(box.max_y, 3);

    auto verts = tri.Vertices();
    EXPECT_EQ(verts.size(), 3);
}

TEST(GeometryTest, Rectangle_Properties) {
    Rectangle rect{{1, 2}, 3, 4};
    EXPECT_DOUBLE_EQ(rect.Height(), 6);

    auto tr = rect.TopRight();
    EXPECT_DOUBLE_EQ(tr.x, 4);
    EXPECT_DOUBLE_EQ(tr.y, 6);

    auto center = rect.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.5);
    EXPECT_DOUBLE_EQ(center.y, 4);

    auto box = rect.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, 1);
    EXPECT_DOUBLE_EQ(box.max_x, 4);
    EXPECT_DOUBLE_EQ(box.min_y, 2);
    EXPECT_DOUBLE_EQ(box.max_y, 6);

    auto verts = rect.Vertices();
    EXPECT_EQ(verts.size(), 4);
    EXPECT_DOUBLE_EQ(verts[0].x, 1);
    EXPECT_DOUBLE_EQ(verts[1].x, 4);
    EXPECT_DOUBLE_EQ(verts[2].x, 4);
    EXPECT_DOUBLE_EQ(verts[3].x, 1);
}

TEST(GeometryTest, Circle_Properties) {
    Circle circle{{0, 0}, 5};
    EXPECT_DOUBLE_EQ(circle.Height(), 5);

    auto center = circle.Center();
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);

    auto box = circle.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -5);
    EXPECT_DOUBLE_EQ(box.max_x, 5);
    EXPECT_DOUBLE_EQ(box.min_y, -5);
    EXPECT_DOUBLE_EQ(box.max_y, 5);

    auto verts = circle.Vertices(4);
    EXPECT_EQ(verts.size(), 4);
    EXPECT_NEAR(verts[0].x, 5, kEpsilon);
    EXPECT_NEAR(verts[0].y, 0, kEpsilon);
}

TEST(GeometryTest, RegularPolygon_Properties) {
    RegularPolygon poly{{0, 0}, 1, 4};
    EXPECT_DOUBLE_EQ(poly.Height(), 1);

    auto center = poly.Center();
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);

    auto box = poly.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -1);
    EXPECT_DOUBLE_EQ(box.max_x, 1);
    EXPECT_DOUBLE_EQ(box.min_y, -1);
    EXPECT_DOUBLE_EQ(box.max_y, 1);

    auto verts = poly.Vertices();
    EXPECT_EQ(verts.size(), 4);
    EXPECT_NEAR(verts[0].x, 1, kEpsilon);
    EXPECT_NEAR(verts[0].y, 0, kEpsilon);
}

TEST(GeometryTest, Polygon_Properties) {
    Polygon poly{{{-1, -1}, {2, 0}, {1, 3}, {-2, 2}}};

    auto box = poly.BoundBox();
    EXPECT_DOUBLE_EQ(box.min_x, -2);
    EXPECT_DOUBLE_EQ(box.max_x, 2);
    EXPECT_DOUBLE_EQ(box.min_y, -1);
    EXPECT_DOUBLE_EQ(box.max_y, 3);

    EXPECT_DOUBLE_EQ(poly.Height(), 4);

    auto center = poly.Center();
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 1);

    auto verts = poly.Vertices();
    EXPECT_EQ(verts.size(), 4);
}