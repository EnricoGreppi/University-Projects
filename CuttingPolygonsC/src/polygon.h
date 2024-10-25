#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <queue>
#include "segment.h"
#include "Eigen"
#include "intersector.h"


using namespace SegmentNamespace;
using namespace std;
using namespace Eigen;
using namespace IntersectorNamespace;

namespace PolygonNamespace {


class Polygon
{
private:
    vector<Vertex> _polygon;
    double tolerance=1.0E-7;
public:

    enum PositionSide
    {
        Left = 0,
        Right = 1,
        Forward = 2,
        Behind= 3,
        Begin=4,
        End=5,
        In=6

    };


    Polygon();
    Polygon(const vector<Vertex> & vertices);
    Polygon(queue<Vertex> & vertices);
    ~Polygon();
    int NumberVertices() const;
    Vertex GetVertex(const int & vertexPosition) const;
    const Segment GetEdge(const int &partenza) const;
    bool TypePolygon(); // true se concavo
    PositionSide SidePoint (const Segment &segment,const Point & point);
    bool PuntoInterno(const Point & punto); //solo per poligoni convessi
    Polygon& operator =(const Polygon& polygon);//T
    friend inline bool operator==(const Polygon& lhs, const Polygon& rhs){ return lhs._polygon==rhs._polygon;}//T

};
};

#endif // POLYGON_H

