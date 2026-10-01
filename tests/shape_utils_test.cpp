#include "geometry.hpp"
#include "shape_utils.hpp"
#include <gtest/gtest.h>

using namespace geometry;
using namespace geometry::utils;

TEST(ShapeUtilsTest, FindAllCollisions_Empty) {
    std::vector<Shape> shapes;
    auto collisions = FindAllCollisions(shapes);
    EXPECT_TRUE(collisions.empty());
}

TEST(ShapeUtilsTest, FindAllCollisions_SingleShape) {
    std::vector<Shape> shapes = {Circle{{0, 0}, 1}};
    auto collisions = FindAllCollisions(shapes);
    EXPECT_TRUE(collisions.empty());
}

TEST(ShapeUtilsTest, FindAllCollisions_NoCollision) {
    std::vector<Shape> shapes = {Circle{{0, 0}, 1}, Circle{{10, 0}, 1}};
    auto collisions = FindAllCollisions(shapes);
    EXPECT_TRUE(collisions.empty());
}

TEST(ShapeUtilsTest, FindAllCollisions_OneCollision) {
    std::vector<Shape> shapes = {Circle{{0, 0}, 2}, Circle{{1, 0}, 2}};
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 1);
}

TEST(ShapeUtilsTest, FindAllCollisions_MultipleCollisions) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 2},
        Circle{{1, 0}, 2},
        Circle{{2, 0}, 2},
    };
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 3);  // (0,1), (0,2), (1,2)
}

TEST(ShapeUtilsTest, FindAllCollisions_DifferentShapes) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 2},
        Rectangle{{0, 0}, 1, 1},
    };
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 1);
}

TEST(ShapeUtilsTest, FindHighestShape_Empty) {
    std::vector<Shape> shapes;
    auto highest = FindHighestShape(shapes);
    EXPECT_FALSE(highest.has_value());
}

TEST(ShapeUtilsTest, FindHighestShape_Single) {
    std::vector<Shape> shapes = {Circle{{0, 0}, 1}};
    auto highest = FindHighestShape(shapes);
    ASSERT_TRUE(highest.has_value());
    EXPECT_EQ(*highest, 0);
}

TEST(ShapeUtilsTest, FindHighestShape_Multiple) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 1},                 // height = 1
        Line{{0, 0}, {0, 5}},              // height = 5
        Triangle{{0, 0}, {1, 0}, {0, 2}},  // height = 2
    };
    auto highest = FindHighestShape(shapes);
    ASSERT_TRUE(highest.has_value());
    EXPECT_EQ(*highest, 1);
}