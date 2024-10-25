#ifndef TEST_CUTTER_H
#define TEST_CUTTER_H


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "Eigen"

#include "cutter.h"

using namespace CutterNamespace;
using namespace testing;
using namespace std;

namespace CutterTesting
{
TEST(TestCutter, CutterMethods)
{
    ConvexCutter test;
    ConcaveCutter test2;
    list<Point> testPoints;
    Polygon testPolygon;
    Segment testCut;
    vector<Point> punti;
    vector<Vertex> creaPoligono;
    punti.resize(4);
    creaPoligono.resize(4);
    punti[0]=Point(1,1);
    punti[1]=Point(5,1);
    punti[2]=Point(5,3.1);
    punti[3]=Point(1,3.1);
    for (int i=0;i<4;i++)
        creaPoligono[i]=Vertex(i,punti[i]);
    testCut=Segment(Point(2,1.2),Point(4,3));
    testPolygon=Polygon(creaPoligono);
    try
    {
        test.SetCutter(punti,creaPoligono,testCut);
        EXPECT_EQ(testCut.GetFrom(),test.GetCut().GetFrom());
        EXPECT_EQ(testCut.GetTo(), test.GetCut().GetTo());
        EXPECT_EQ(test.GetPolygon(0), testPolygon);

        test.SetTolerance(0.0000000001);
        EXPECT_EQ(test.GetTolerance(),0.0000000001);
    }
    catch (const exception &exception) {
        FAIL();
    }
    try
    {
        test2.SetCutter(punti,creaPoligono,testCut);

        EXPECT_TRUE(test2.SearchPolygon(testPolygon));

        EXPECT_EQ(testCut.GetFrom(),test2.GetCut().GetFrom());
        EXPECT_EQ(testCut.GetTo(), test2.GetCut().GetTo());


        test2.SetTolerance(0.0000000001);
        EXPECT_EQ(test2.GetTolerance(),0.0000000001);


    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure1)
{
    ConvexCutter prova;//=ConvexCutter();
    vector<Point> test;
    vector<Vertex> vertici;
    test.reserve(4);
    vertici.reserve(4);
    test.push_back(Point(1,1));
    test.push_back(Point(5,1));
    test.push_back(Point(5,3.1));
    test.push_back(Point(1,3.1));
    for (int i=0;i<4;i++)
        vertici.push_back(Vertex(i,test[i]));
    Polygon testDestra;
    Polygon testSinistra;
    vector<Vertex> verticiD;
    vector<Vertex> verticiS;
    verticiS.resize(6);
    verticiD.resize(6);
    verticiS[0]=Vertex(0,test[0]);
    verticiS[1]=Vertex(4,Point(1.7777777777777777,1.0));
    verticiS[2]=Vertex(5,Point(2,1.2));
    verticiS[3]=Vertex(6,Point(4,3));
    verticiS[4]=Vertex(7,Point(4.111111111111111,3.1000000000000005));
    verticiS[5]=Vertex(3,test[3]);

    verticiD[0]=Vertex(6,Point(4,3));
    verticiD[1]=Vertex(5,Point(2,1.2));
    verticiD[2]=(Vertex(4,Point(1.7777777777777777,1.0)));
    verticiD[3]=(Vertex(1,test[1]));
    verticiD[4]=(Vertex(2,test[2]));
    verticiD[5]=Vertex(7,Point(4.111111111111111,3.1000000000000005));

    testSinistra=Polygon(verticiS);
    testDestra=Polygon(verticiD);
    Segment cutter;
    cutter=Segment(Point(2,1.2),Point(4,3));
    //prova.SetCutter(test,vertici,cutter);
    prova.CutPolygon(test,vertici,cutter);
    try {
        Polygon sinistra;
        Polygon destra;
        destra=prova.GetPolygon(1);
        sinistra=prova.GetPolygon(2);

       EXPECT_EQ(sinistra,testSinistra);
       EXPECT_EQ(destra,testDestra);


    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure2)
{
    ConvexCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    vector<Vertex> crea;
    test.resize(4);
    crea.resize(5);
    vertici.resize(4);
    test[0]=Point(1,1);
    test[1]=Point(5,1);
    test[2]=Point(5,3.1);
    test[3]=Point(1,3.1);
    for (int i=0;i<4;i++)
        vertici[i]=Vertex(i,test[i]);
    crea[0]=Vertex(0,test[0]);
    crea[1]=Vertex(4,Point(2,1));
    crea[2]=Vertex(1,test[1]);
    crea[3]=Vertex(2,test[2]);
    crea[4]=Vertex(3,test[3]);

    Polygon uguale;

    uguale=Polygon(crea);
    Segment cutter;
    cutter=Segment(Point(2,1),Point(6,1));
    prova.CutPolygon(test,vertici,cutter);
    try {
        Polygon polig1;
        polig1=prova.GetPolygon(1);

       EXPECT_EQ(polig1,uguale);
       EXPECT_EQ(prova.cambi,10);


    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure3)
{
    ConvexCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    test.resize(5);
    vertici.resize(5);
    test[0]=Point(2.5,1);
    test[1]=Point(4,2.1);
    test[2]=Point(3.4,4.2);
    test[3]=Point(1.6,4.2);
    test[4]=Point(1,2.1);
    for (int i=0;i<5;i++)
        vertici[i]=Vertex(i,test[i]);
    Polygon testDestra;
    Polygon testSinistra;
    vector<Vertex> verticiD;
    vector<Vertex> verticiS;
    verticiS.resize(6);
    verticiD.resize(6);
    verticiS[0]=Vertex(0,test[0]);
    verticiS[1]=Vertex(1,test[1]);//intersezione
    verticiS[2]=Vertex(5,Point(3.6,2.2));
    verticiS[3]=Vertex(6,Point(1.4,2.75));
    verticiS[4]=Vertex(7,Point(1.2000000000000002,2.8));
    verticiS[5]=Vertex(4,test[4]);

    verticiD[0]=Vertex(6,Point(1.4,2.75));
    verticiD[1]=Vertex(5,Point(3.6,2.2));
    verticiD[2]=Vertex(1,test[1]);//intersezione
    verticiD[3]=Vertex(2,test[2]);
    verticiD[4]=Vertex(3,test[3]);
    verticiD[5]=Vertex(7,Point(1.2000000000000002,2.8));

    testSinistra=Polygon(verticiS);
    testDestra=Polygon(verticiD);
    Segment cutter;
    cutter=Segment(Point(1.4,2.75),Point(3.6,2.2));
    prova.CutPolygon(test,vertici,cutter);
    try {
        Polygon sinistra;
        Polygon destra;
        destra=prova.GetPolygon(1);
        sinistra=prova.GetPolygon(2);

       EXPECT_EQ(sinistra,testSinistra);
       EXPECT_EQ(destra,testDestra);


       EXPECT_EQ(sinistra.GetVertex(0),verticiS[0]);
       EXPECT_EQ(sinistra.GetVertex(1),verticiS[1]);
       EXPECT_EQ(sinistra.GetVertex(2),verticiS[2]);
       EXPECT_EQ(sinistra.GetVertex(3),verticiS[3]);

       EXPECT_EQ(destra.GetVertex(0),verticiD[0]);
       EXPECT_EQ(destra.GetVertex(1),verticiD[1]);
       EXPECT_EQ(destra.GetVertex(2),verticiD[2]);
       EXPECT_EQ(destra.GetVertex(3),verticiD[3]);

    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure4)
{
    ConcaveCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    test.resize(6);
    vertici.resize(6);
    test[0]=Point(1.5,1);
    test[1]=Point(5.6,1.5);
    test[2]=Point(5.5,4.8);
    test[3]=Point(4.0,6.2);
    test[4]=Point(3.2,4.2);
    test[5]=Point(1.0,4.0);
    for (int i=0;i<6;i++)
        vertici[i]=Vertex(i,test[i]);
    Segment cutter;
    cutter=Segment(Point(2.0,3.7),Point(4.1,5.9));
    Polygon testP1;
    Polygon testP2;
    Polygon testP3;
    vector<Vertex> Polygon1;
    vector<Vertex> Polygon2;
    vector<Vertex> Polygon3;
    Polygon1.resize(10);
    Polygon2.resize(4);
    Polygon3.resize(4);

    Polygon1[0]=vertici[0];
    Polygon1[1]=vertici[1];
    Polygon1[2]=vertici[2];
    Polygon1[3]=Vertex(6, Point(4.2043269230769225,6.009294871794872));
    Polygon1[4]=Vertex(7,cutter.GetTo());
    Polygon1[5]=Vertex(8, Point(3.7213114754098364,5.503278688524591));
    Polygon1[6]=vertici[4];
    Polygon1[7]=Vertex(9,Point(2.408597285067873,4.128054298642534));
    Polygon1[8]=Vertex(10,cutter.GetFrom());
    Polygon1[9]=Vertex(11,Point(1.1912162162162163,2.852702702702703));
    testP1=Polygon(Polygon1);

    Polygon2[0]=vertici[3];
    Polygon2[1]=Vertex(8, Point(3.7213114754098364,5.503278688524591));
    Polygon2[2]=Vertex(7,cutter.GetTo());
    Polygon2[3]=Vertex(6, Point(4.2043269230769225,6.009294871794872));
    testP2=Polygon(Polygon2);

    Polygon3[0]=vertici[5];
    Polygon3[1]=Vertex(11,Point(1.1912162162162163,2.852702702702703));
    Polygon3[2]=Vertex(10,cutter.GetFrom());
    Polygon3[3]=Vertex(9,Point(2.408597285067873,4.128054298642534));
    testP3=Polygon(Polygon3);

    prova.CutPolygon(test,vertici,cutter);

    bool trovato1;
    bool trovato2;
    bool trovato3;
    try {
        trovato1=prova.SearchPolygon(testP1);
        trovato2=prova.SearchPolygon(testP2);
        trovato3=prova.SearchPolygon(testP3);

        EXPECT_TRUE(trovato1);
        EXPECT_TRUE(trovato2);
        EXPECT_TRUE(trovato3);

    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure5)
{
    ConcaveCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    test.resize(6);
    vertici.resize(6);
    test[0]=Point(-5,4);
    test[1]=Point(0,0);
    test[2]=Point(3,0);
    test[3]=Point(-2,4);
    test[4]=Point(3,6);
    test[5]=Point(0,8);
    for (int i=0;i<6;i++)
        vertici[i]=Vertex(i,test[i]);
    Segment cutter;
    cutter=Segment(Point(0,0.5),Point(0,8));
    Polygon testP1;
    Polygon testP2;
    Polygon testP3;
    vector<Vertex> Polygon1;
    vector<Vertex> Polygon2;
    vector<Vertex> Polygon3;
    Polygon1.resize(7);
    Polygon2.resize(4);
    Polygon3.resize(3);

    Polygon1[0]=vertici[0];
    Polygon1[1]=vertici[1];
    Polygon1[2]=Vertex(6,cutter.GetFrom());
    Polygon1[3]=Vertex(7, Point(2.220446049250313e-16,2.4));
    Polygon1[4]=vertici[3];
    Polygon1[5]=Vertex(8, Point(2.220446049250313e-16,4.800000000000001));
    Polygon1[6]=vertici[5];
    testP1=Polygon(Polygon1);

    Polygon2[0]=vertici[2];
    Polygon2[1]=Vertex(7, Point(2.220446049250313e-16,2.4));
    Polygon2[2]=Vertex(6,cutter.GetFrom());
    Polygon2[3]=vertici[1];
    testP2=Polygon(Polygon2);

    Polygon3[0]=vertici[4];
    Polygon3[1]=vertici[5];
    Polygon3[2]=Vertex(8,Point(2.220446049250313e-16,4.800000000000001));
    testP3=Polygon(Polygon3);

    prova.CutPolygon(test,vertici,cutter);

    bool trovato1;
    bool trovato2;
    bool trovato3;
    try {
        trovato1=prova.SearchPolygon(testP1);
        trovato2=prova.SearchPolygon(testP2);
        trovato3=prova.SearchPolygon(testP3);

        EXPECT_TRUE(trovato1);
        EXPECT_TRUE(trovato2);
        EXPECT_TRUE(trovato3);

    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, CutterFigure6)
{
    ConcaveCutter prova;
    vector<Point> test;
    vector<Vertex> vertici;
    test.resize(7);
    vertici.resize(7);
    test[0]=Point(0,0);
    test[1]=Point(0,8);
    test[2]=Point(-5,8);
    test[3]=Point(-2,3);
    test[4]=Point(-5,3);
    test[5]=Point(-5,6);
    test[6]=Point(-8,0);
    for (int i=0;i<7;i++)
        vertici[i]=Vertex(i,test[i]);
    Segment cutter;
    cutter=Segment(Point(-3,3),Point(-4,3));
    Polygon testP1;
    Polygon testP2;
    Polygon testP3;
    vector<Vertex> Polygon1;
    vector<Vertex> Polygon2;
    vector<Vertex> Polygon3;
    Polygon1.resize(8);
    Polygon2.resize(4);
    Polygon3.resize(3);

    Polygon1[0]=vertici[0];
    Polygon1[1]=Vertex(7,Point(0.0,3.0));
    Polygon1[2]=vertici[3];
    Polygon1[3]=Vertex(8, cutter.GetFrom());
    Polygon1[4]=Vertex(9,cutter.GetTo());
    Polygon1[5]=vertici[4];
    Polygon1[6]=Vertex(10,Point(-6.5,3));
    Polygon1[7]=vertici[6];
    testP1=Polygon(Polygon1);

    Polygon2[0]=vertici[1];
    Polygon2[1]=vertici[2];
    Polygon2[2]=vertici[3];
    Polygon2[3]=Vertex(7,Point(0.0,3.0));
    testP2=Polygon(Polygon2);

    Polygon3[0]=vertici[5];
    Polygon3[1]=Vertex(10,Point(-6.5,3));
    Polygon3[2]=vertici[4];
    testP3=Polygon(Polygon3);

    prova.CutPolygon(test,vertici,cutter);

    bool trovato1;
    bool trovato2;
    bool trovato3;
    try {
        trovato1=prova.SearchPolygon(testP1);
        trovato2=prova.SearchPolygon(testP2);
        trovato3=prova.SearchPolygon(testP3);

        EXPECT_TRUE(trovato1);
        EXPECT_TRUE(trovato2);
        EXPECT_TRUE(trovato3);

    }
    catch (const exception &exception) {
        FAIL();
    }
}
TEST(TestCutter, Testaggiuntivo1)
{
    ConcaveCutter prova;
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
    cutter=Segment(Point(0,-3),Point(0,4));
    Polygon testP1;
    Polygon testP2;
    Polygon testP3;
    Polygon testP4;
    Polygon testP5;
    vector<Vertex> Polygon1;
    vector<Vertex> Polygon2;
    vector<Vertex> Polygon3;
    vector<Vertex> Polygon4;
    vector<Vertex> Polygon5;
    Polygon1.resize(3);
    Polygon2.resize(4);
    Polygon3.resize(4);
    Polygon4.resize(5);
    Polygon5.resize(4);

    Polygon1[0]=vertici[0];
    Polygon1[1]=vertici[1];
    Polygon1[2]=Vertex(10, Point(-2.220446049250313e-16,-2));


    testP1=Polygon(Polygon1);

    Polygon2[0]=vertici[2];
    Polygon2[1]=vertici[3];
    Polygon2[2]=vertici[8];
    Polygon2[3]=vertici[1];

    testP2=Polygon(Polygon2);

    Polygon3[0]=vertici[4];
    Polygon3[1]=vertici[5];
    Polygon3[2]=Vertex(11,Point(0,3.0));
    Polygon3[3]=vertici[3];
    testP3=Polygon(Polygon3);

    Polygon4[0]=vertici[6];
    Polygon4[1]=vertici[7];
    Polygon4[2]=vertici[8];
    Polygon4[3]=vertici[3];
    Polygon4[4]=Vertex(11,Point(0,3.0));
    testP4=Polygon(Polygon4);

    Polygon5[0]=vertici[9];
    Polygon5[1]=Vertex(10, Point(-2.220446049250313e-16,-2));
    Polygon5[2]=vertici[1];
    Polygon5[3]=vertici[8];
    testP5=Polygon(Polygon5);

    prova.CutPolygon(test,vertici,cutter);

    bool trovato1;
    bool trovato2;
    bool trovato3;
    bool trovato4;
    bool trovato5;
    try {
        trovato1=prova.SearchPolygon(testP1);
        trovato2=prova.SearchPolygon(testP2);
        trovato3=prova.SearchPolygon(testP3);
        trovato4=prova.SearchPolygon(testP4);
        trovato5=prova.SearchPolygon(testP5);

        EXPECT_TRUE(trovato1);
        EXPECT_TRUE(trovato2);
        EXPECT_TRUE(trovato3);
        EXPECT_TRUE(trovato4);
        EXPECT_TRUE(trovato5);
    }
    catch (const exception &exception) {
        FAIL();
    }
}
}


#endif // TEST_CUTTER_H
