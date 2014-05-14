#include "gtest/gtest.h"
#include "matrixops.h"

TEST(CollisionTest, collision_test)
{
  ofRectangle rect1(0,0,100,100);
  ofRectangle rect2(150,150,100,100);
  ASSERT_EQ(true, MatrixOperations::doesCollide(rect1, rect2));
}
