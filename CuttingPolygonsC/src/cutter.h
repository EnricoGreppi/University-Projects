#ifndef CUTTER_H
#define CUTTER_H
#include <iostream>
#include <list>
#include<queue>
#include <unordered_map>
#include <vector>
#include "Eigen"
#include "polygon.h"
#include "intersector.h"


using namespace PolygonNamespace;
using namespace IntersectorNamespace;
using namespace std;
using namespace Eigen;


namespace CutterNamespace {

class Icutter
{
    virtual void CutPolygon (const vector<Point> & points, const vector<Vertex> & polygonVertices, const Segment &segment)=0;
    virtual void SetCutter (const vector<Point> & points,const vector<Vertex> & vertex,const Segment &segment)=0;
    virtual void SetTolerance( const double & toll)=0;
    virtual Segment GetCut() const=0;
    virtual double GetTolerance()=0;
};

class ConvexCutter:public Icutter
{
private:
    list<Point> _points;
    vector<Polygon> _polygons;
    Segment _cut;
    double tolerance=1.0E-7;
public:

    queue<Vertex> rightSide;
    queue<Vertex> leftSide;
    Intersector intersector;
    int cambi;



    ConvexCutter();
    ~ConvexCutter();
    void SetCutter(const vector<Point> &points,const vector<Vertex>& vertex,const Segment &segment);
    void SetTolerance( const double & toll);
    double GetTolerance();
    Segment GetCut() const;
    Polygon GetPolygon(const int & i) const;
    void CutPolygon(const vector<Point> & points,const vector<Vertex> & polygonVertices,const Segment &segment);
    void SetPolygon();

};


class ConcaveCutter:public Icutter
{
private:
    list<Point> _points;
    list<Polygon> _polygons;
    Segment _cut;
    double tolerance=1.0E-7;
public:

    Intersector intersector;
    list<int> inseriti; //inserito in qualche poligono
    list<int> inseritiAppoggio; //poligono in costruzione ora
    list<Vertex> nuoviVertici;

    queue<Vertex> appoggio;  //per poligono in costruzione ora

    ConcaveCutter();
    ~ConcaveCutter();
    void SetCutter(const vector<Point> & points,const vector<Vertex>& vertex,const Segment &segment);
     void SetTolerance( const double & toll);
     Segment GetCut() const;
     double GetTolerance();




     void CutPolygon(const vector<Point> &points, const  vector<Vertex> &polygonVertices, const Segment & segment);//T

     int GiustoSegmento(const Segment &intersecato,const Polygon &oldP ,const Point &intersezione);

     bool SearchInseriti(const int& id);

     bool SearchInseritiAppoggio(const int& id);

     Vertex GiustoVertice (const Vertex &controllo);
     bool SearchVertici(const Vertex& controllo);

     bool SearchPolygon(const Polygon& poligono);//T

     int ControlloSegmento(const Segment &intersecato1,const Segment & intersecato2, const Segment & cutter);
     int ControlloSegmento2(const Segment &lato,const Segment &cutter); //per i casi paralleli

};
}
#endif // CUTTER_H
