#ifndef TEST_SEGMENT_H
#define TEST_SEGMENT_H
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "Eigen"

#include "segment.h"

using namespace SegmentNamespace;
using namespace testing;
using namespace std;

namespace SementTesting
{
TEST(testSegment,segmentMethods)
{
    Segment test;
    Point a;
    Point b;
    a=Point(1,2);
    b=Point(5,2);

    test=Segment(a,b);
    try
    {
        EXPECT_EQ(test.GetFrom(),a);
        EXPECT_EQ(test.GetTo(),b);
    }
    catch (exception &exception) {
        FAIL();
    }
    try
    {
        Point  left, right, in, forward, behind, end, begin;
        left=Point(0,5);
        right=Point(3,0);
        end=Point(5,2);
        in =Point(3,2);
        begin=Point(1,2);
        forward=Point(7,2);
        behind=Point(0,2);
        EXPECT_EQ(test.SidePoint(left),Segment::Left);
        EXPECT_EQ(test.SidePoint(right),Segment::Right);
        EXPECT_EQ(test.SidePoint(in),Segment::In);
        EXPECT_EQ(test.SidePoint(end),Segment::End);
        EXPECT_EQ(test.SidePoint(begin),Segment::Begin);
        EXPECT_EQ(test.SidePoint(behind),Segment::Behind);
        EXPECT_EQ(test.SidePoint(forward),Segment::Forward);
    }
    catch (exception &exception) {
        FAIL();
    }
}
}


#endif // TEST_SEGMENT_H
