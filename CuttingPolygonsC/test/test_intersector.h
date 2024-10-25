#ifndef __TEST_INTERSECTOR_H
#define __TEST_INTERSECTOR_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "intersector.h"

using namespace testing;
using namespace IntersectorNamespace;

namespace IntersectorTesting {

TEST(TestIntersector, IntersectorMethods)
{
  Point a,b,c,d;
  a=Point(0, 0);
  b=Point(4, 0);
  c=Point(1, 0);
  d=Point(2, 0);
  Segment ab;
  Segment cd;
  ab=Segment(a,b);
  cd=Segment(c,d);

  Intersector intersectorNew;
  try {
  intersectorNew.SetCutter(ab);
  intersectorNew.SetEdge(cd);
  EXPECT_TRUE(intersectorNew.ComputeIntersection());
  EXPECT_EQ(intersectorNew.TypeIntersection(), Intersector::IntersectionParallelOnSegment);
  EXPECT_FLOAT_EQ(0.25, intersectorNew.FirstParametricCoordinate());
  EXPECT_FLOAT_EQ(0.5, intersectorNew.SecondParametricCoordinate());
  }
  catch(exception &exception){
      FAIL();
  }

   a=Point(1, 0);
   b=Point(5, 0);
   c=Point(3, -6);
   d=Point(3, 6);
   ab=Segment(a,b);
   cd = Segment(c,d);
  Intersector intersector;
  try {
  intersector.SetCutter(ab);
  intersector.SetEdge(cd);
  EXPECT_TRUE(intersector.ComputeIntersection());
  EXPECT_EQ(intersector.TypeIntersection(), Intersector::IntersectionOnSegment);
  EXPECT_EQ(intersector.PositionIntersectionEdge(),Intersector::Inner);
  EXPECT_FLOAT_EQ(0.5, intersector.SecondParametricCoordinate());
  EXPECT_FLOAT_EQ(0.5, intersector.FirstParametricCoordinate());
  EXPECT_LE(intersector.FirstParametricCoordinate(), 1.0);
  EXPECT_GE(intersector.FirstParametricCoordinate(), 0.0);
  EXPECT_EQ(Point(intersector.IntersectionCutterParametricCoordinate(ab)),Point(intersector.IntersectionEdgeParametricCoordinate(cd)));
  }
  catch (exception &exception){
      FAIL();
  }
  a=Point(3, 6);
  b=Point(3, 2);
  c=Point(5, 0);
  d=Point(1, 0);
  ab=Segment(a,b);
  cd=Segment(c,d);

  Intersector intersector3;
  try {
  intersector3.SetCutter(ab);
  intersector3.SetEdge(cd);
  EXPECT_TRUE(intersector3.ComputeIntersection());
  EXPECT_EQ(intersector3.TypeIntersection(), Intersector::IntersectionOnLine);
  EXPECT_EQ(intersector3.TypeIntersection(), 1);
  EXPECT_FLOAT_EQ(1.5, intersector3.FirstParametricCoordinate());
  EXPECT_FLOAT_EQ(0.5, intersector3.SecondParametricCoordinate());
  EXPECT_GE(intersector3.FirstParametricCoordinate(), 1.0);
  }
  catch(exception &exception){
      FAIL();
  }
  }
}

#endif // __TEST_INTERSECTOR1D1D_H

