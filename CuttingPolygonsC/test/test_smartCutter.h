#ifndef TEST_SMARTCUTTER_H
#define TEST_SMARTCUTTER_H
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "smartcutter.h"

using namespace testing;
using namespace smartCutterNamespace;
TEST(TestSmartCutter, Testaggiuntivo2)
{
    SmartCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    test.resize(10);
    vertici.resize(10);
    test[0]=Point(2,-2);
    test[1]=Point(0,-1);
    test[2]=Point(3,1);
    test[3]=Point(0,2);
    test[4]=Point(3,2);
    test[5]=Point(3,3);
    test[6]=Point(-1,3);
    test[7]=Point(-3,1);
    test[8]=Point(0,0);
    test[9]=Point(-3,-2);
    for (int i=0;i<10;i++)
        vertici[i]=Vertex(i,test[i]);
    Segment cutter;
    cutter=Segment(Point(-4,-4),Point(4,4));
    Polygon testP1;
    Polygon testP2;
    Polygon testP3;
    Polygon testP4;
    vector<Vertex> Polygon1;
    vector<Vertex> Polygon2;
    vector<Vertex> Polygon3;
    vector<Vertex> Polygon4;
    Polygon1.resize(6);
    Polygon2.resize(7);
    Polygon3.resize(3);
    Polygon4.resize(3);

    Polygon1[0]=vertici[0];
    Polygon1[1]=vertici[1];
    Polygon1[2]=vertici[2];
    Polygon1[3]=Vertex(10, Point(1.5,1.5));
    Polygon1[4]=vertici[8];
    Polygon1[5]=Vertex(11,Point(-2.0000000000000004,-2.0));

    testP1=Polygon(Polygon1);

    Polygon2[0]=vertici[3];
    Polygon2[1]=Vertex(12,Point(2.0,2.0));
    Polygon2[2]=vertici[5];
    Polygon2[3]=vertici[6];
    Polygon2[4]=vertici[7];
    Polygon2[5]=vertici[8];
    Polygon2[6]=Vertex(10,Point(1.5,1.5));
    testP2=Polygon(Polygon2);

    Polygon3[0]=vertici[4];
    Polygon3[1]=vertici[5];
    Polygon3[2]=Vertex(12,Point(2.0,2.0));
    testP3=Polygon(Polygon3);

    Polygon4[0]=vertici[9];
    Polygon4[1]=Vertex(11,Point(-2.0000000000000004,-2.0));
    Polygon4[2]=vertici[8];
    testP4=Polygon(Polygon4);

    prova=SmartCutter(test,vertici,cutter);

    bool trovato1;
    bool trovato2;
    bool trovato3;
    bool trovato4;
    try {
        trovato1=prova._concavo.SearchPolygon(testP1);
        trovato2=prova._concavo.SearchPolygon(testP2);
        trovato3=prova._concavo.SearchPolygon(testP3);
        trovato4=prova._concavo.SearchPolygon(testP4);

        EXPECT_TRUE(trovato1);
        EXPECT_TRUE(trovato2);
        EXPECT_TRUE(trovato3);
        EXPECT_TRUE(trovato4);
    }
    catch (const exception &exception) {
        FAIL();
    }
}
#endif // TEST_SMARTCUTTER_H
