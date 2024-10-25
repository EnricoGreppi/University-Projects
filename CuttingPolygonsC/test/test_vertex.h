#ifndef TEST_VERTEX_H
#define TEST_VERTEX_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "Eigen"

#include "vertex.h"

using namespace VertexNamespace;
using namespace testing;
using namespace std;

namespace VertexTesting
{
TEST(TestVertex, VertexMethods)
{
    Vertex vertex;
    Vector2d coordinate;
    coordinate(0)=2;
    coordinate(1)=3;
    vertex=Vertex(1,coordinate);

    try
    {
        int test=1;
        EXPECT_EQ(vertex.Id,test);
    }
    catch (exception &exception) {
        FAIL();
    }
    try
    {
        Point test;
        Point testVertex;
        Vertex testeq;
        testeq=vertex;
        test=Point(2,3);
        EXPECT_EQ(testeq,vertex);
        testVertex=vertex.GetPoint();
        EXPECT_EQ(testVertex,test);
        EXPECT_EQ(vertex.GetX(),coordinate(0));
        EXPECT_EQ(vertex.GetY(),coordinate(1));


    }
    catch (exception &exception) {
        FAIL();
    }
}
}

#endif // TEST_VERTEX_H
