#ifndef TEST_POLYGON_H
#define TEST_POLYGON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "polygon.h"

using namespace testing;
using namespace PolygonNamespace;

namespace PolygonTesting {

void FillPolygonVertices(vector<Vertex>& vertices)
{
  vertices.reserve(10);
  vertices.push_back(Vertex(0,1.0000e+00, 2.0000e+00));
  vertices.push_back(Vertex(1,7.6489e-01, 1.3236e+00));
  vertices.push_back(Vertex(2,4.8943e-02, 1.3090e+00));
  vertices.push_back(Vertex(3,6.1958e-01, 8.7639e-01));
  vertices.push_back(Vertex(4,4.1221e-01, 1.9098e-01));
  vertices.push_back(Vertex(5,1.0000e+00, 6.0000e-01));
  vertices.push_back(Vertex(6,1.5878e+00, 1.9098e-01));
  vertices.push_back(Vertex(7,1.3804e+00, 8.7639e-01));
  vertices.push_back(Vertex(8,1.9511e+00, 1.3090e+00));
  vertices.push_back(Vertex(9,1.2351e+00, 1.3236e+00));
}
void FillQueue(queue<Vertex> & vertices)
{
  vertices.push(Vertex(0,1.0000e+00, 2.0000e+00));
  vertices.push(Vertex(1,7.6489e-01, 1.3236e+00));
  vertices.push(Vertex(2,4.8943e-02, 1.3090e+00));
  vertices.push(Vertex(3,6.1958e-01, 8.7639e-01));
  vertices.push(Vertex(4,4.1221e-01, 1.9098e-01));
  vertices.push(Vertex(5,1.0000e+00, 6.0000e-01));
  vertices.push(Vertex(6,1.5878e+00, 1.9098e-01));
  vertices.push(Vertex(7,1.3804e+00, 8.7639e-01));
  vertices.push(Vertex(8,1.9511e+00, 1.3090e+00));
  vertices.push(Vertex(9,1.2351e+00, 1.3236e+00));
}

TEST(TestPolygon, PolygonMethods)
{
    vector<Vertex> vertices;
    queue<Vertex> coda;
    FillPolygonVertices(vertices);
    FillQueue(coda);
    Polygon test;
    Polygon testCoda;
    testCoda=Polygon(coda);
    test=Polygon(vertices);
    Polygon testconcavo;
    Polygon testconvesso;
    vector<Vertex> concave;
    vector<Vertex> convex;
    convex.reserve(4);
    convex.push_back(Vertex(0,0,0));
    convex.push_back(Vertex(1,10,0));
    convex.push_back(Vertex(2,10,5));
    convex.push_back(Vertex(3,0,5));
    concave.reserve (5);
    concave.push_back(Vertex(0,0,0));
    concave.push_back(Vertex(1,10,0));
    concave.push_back(Vertex(2,10,5));
    concave.push_back(Vertex(3,2,2));
    concave.push_back(Vertex(4,0,5));

    testconvesso = Polygon(convex);
    testconcavo=Polygon(concave);

    try
    {
        EXPECT_EQ(test.NumberVertices(),10);
        EXPECT_EQ(testCoda.NumberVertices(),10);
    }
    catch (exception &exception) {
        FAIL();
    }

    try {
        Polygon testEq;
        testEq=test;
       EXPECT_EQ(test,Polygon(vertices));
       int n;
       n=vertices.size();
       for (int i=0; i<n;i++)
       {
           EXPECT_EQ(vertices[i],test.GetVertex(i));
           EXPECT_EQ(vertices[i],testCoda.GetVertex(i));
       }
       EXPECT_EQ(test,testEq);
       EXPECT_EQ(test,testCoda);
    }
    catch (exception &exception) {
        FAIL();
    }
    try
    {
        Segment prova;
        prova=test.GetEdge(2);
        EXPECT_EQ(prova.GetFrom(),Point(4.8943e-02,1.3090e+00));

        EXPECT_EQ(prova.GetTo(),Point(6.1958e-01,8.7639e-01));

        Segment prova2;
        prova2=test.GetEdge(9);
        EXPECT_EQ(prova2.GetFrom(),Point(1.2351e-00,1.3236+00));

        EXPECT_EQ(prova2.GetTo(),Point(1.0000e-00,2.0000e-00));


    }
    catch (exception &exception) {
        FAIL();
    }

   try {
        EXPECT_FALSE(testconvesso.TypePolygon());
    }
    catch (exception &exception) {
        FAIL();
    }

    try {
        EXPECT_TRUE(testconcavo.TypePolygon());
    }
    catch (exception &exception) {
        FAIL();
    }
    try {
        Point  left, right, in, forward, behind, end, begin,a,b;
        a=Point(1,2);
        b=Point(5,2);
        Segment testSide=Segment(a,b);
        left=Point(0,5);
        right=Point(3,0);
        end=Point(5,2);
        in =Point(3,2);
        begin=Point(1,2);
        forward=Point(7,2);
        behind=Point(0,2);
        EXPECT_EQ(testconvesso.SidePoint(testSide,left),Polygon::Left);
        EXPECT_EQ(testconvesso.SidePoint(testSide,right),Polygon::Right);
        EXPECT_EQ(testconvesso.SidePoint(testSide,in),Polygon::In);
        EXPECT_EQ(testconvesso.SidePoint(testSide,end),Polygon::End);
        EXPECT_EQ(testconvesso.SidePoint(testSide,begin),Polygon::Begin);
        EXPECT_EQ(testconvesso.SidePoint(testSide,behind),Polygon::Behind);
        EXPECT_EQ(testconvesso.SidePoint(testSide,forward),Polygon::Forward);
    }
    catch (exception &exception) {
        FAIL();
    }
    try {
        Point int1,int2,out;
        int1=Point(1,1);
        int2=Point(5,0);
        out=Point(10,-2);
        EXPECT_TRUE(testconvesso.PuntoInterno(int1));
        EXPECT_TRUE(testconvesso.PuntoInterno(int2));
        EXPECT_FALSE(testconvesso.PuntoInterno(out));
    }
    catch (exception &exception) {
        FAIL();
    }

}
}
#endif // TEST_POLYGON_H
