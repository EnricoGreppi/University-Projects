#ifndef __TEST_POINT_H
#define __TEST_POINT_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "Eigen"

#include "point.hpp"

using namespace PointNamespace;
using namespace testing;
using namespace std;

namespace PointTesting {

  TEST(TestPoint, PointMethods)
  {
    Point point;
    Vector2d coordinate;
    coordinate(0)=1;
    coordinate(1)=2;
    point=Point(coordinate);
    try
    {
            double testX=1;
            EXPECT_EQ(point.GetX(),testX);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
        double testY=2;
        EXPECT_EQ(point.GetY(),testY);
    }
    catch (const exception& exception) {
        FAIL();
    }

    try
    {
        double testNorma=sqrt(5);
        EXPECT_EQ(point.ComputeNorm2(),testNorma);
    }
    catch (const exception& exception) {
        FAIL();
    }

    try
    {
        Vector2d test(1,2);
        EXPECT_EQ(point.getCoordinate(), test);
    }
    catch (const exception &exception)
    {
        FAIL();
    }
    try {
        Point testeq;
        testeq=point;    //così ho testato gli operator
        EXPECT_EQ(point,testeq);
    }  catch (exception&exception) {
        FAIL();
    }
    try {
        Point testNorm;
        testNorm=Point(1,3);
        EXPECT_EQ(1,point.ComputeNorm2(testNorm));
    }  catch (exception&exception) {
        FAIL();
    }
  }
}

#endif
