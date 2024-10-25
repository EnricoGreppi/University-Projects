#include "polygon.h"

namespace PolygonNamespace
{

Polygon::Polygon()
{
    _polygon.resize(3);
    for(int i=0; i<3;i++)
    {
        _polygon[i]=Vertex();
    }
}

Polygon::Polygon(const vector<Vertex> &vertices)
{
   int num;
   num=vertices.size();
   _polygon.resize(num);
   for (int i=0; i<num; i++)
       _polygon[i]=vertices[i];
}

Polygon::Polygon(queue<Vertex> &vertices)
{
    int num;
    num=vertices.size();
    _polygon.resize(num);
    for (int i=0; i<num; i++)
       {
        _polygon[i]=vertices.front();
        vertices.pop();
    }
}

Polygon::~Polygon()
{

}


int Polygon::NumberVertices() const
{
    return _polygon.size();
}

Vertex Polygon::GetVertex(const int &vertexPosition)const
{
    return _polygon[vertexPosition];
}

const Segment Polygon::GetEdge( const int &partenza)const
{
    Point a;
    Point da;
    da= _polygon[partenza].GetPoint();
    if (partenza==NumberVertices()-1)
        a=_polygon[0].GetPoint();
    else
        a=_polygon[partenza+1].GetPoint();
    Segment segment;
    segment=Segment(da,a);
    return segment;
}

bool Polygon::TypePolygon()
{
    Intersector intersection;
    intersection = Intersector();
    bool a=0; //convesso
    int N = NumberVertices();
    for(int i=1; i< N && a==0 ; i++ )
        {
        for(int j=(i+2)%N ; j!= (i-1)%N && a==0; j=(j+1)%N)
            {
            intersection.SetCutter(Segment(_polygon[i-1].GetPoint(),_polygon[i].GetPoint()));
            intersection.SetEdge(Segment(_polygon[((j-1)%N+N)%N].GetPoint(),_polygon[j%N].GetPoint())); //se c'è intersectionOnLine tra due lati non consecutivi è concavo
            bool ris= intersection.ComputeIntersection();
           if (ris==true && (intersection.TypeIntersection()==Intersector::IntersectionOnLine)) {

                 a=1;//concavo

            }
           }

        }
    if (a==1)
        return true; //concavo
    else
        return false; //convesso

}

Polygon::PositionSide Polygon::SidePoint(const Segment &segment, const Point &point)
{
    PositionSide position;
    position=Left;
    Point p2;
    Point p0;
    Point p1;
    p0=segment.GetFrom();
    p1=segment.GetTo();
    p2=point;
    Point v;
    Point w;
    Point k;
    v=Point(p1.GetX()-p0.GetX(), p1.GetY()-p0.GetY());
    w=Point(p2.GetX()-p0.GetX(), p2.GetY()-p0.GetY());
    k=Point (p2.GetX()-p1.GetX(), p2.GetY()-p1.GetY());
    Matrix2d controllo;
    controllo.col(0)=v.getCoordinate();
    controllo.col(1)=w.getCoordinate();
    double z;
    z= controllo.determinant();
    if (z>tolerance)
    {
        position=Left;
        return position;
    }
    else if (z<-tolerance)
    {
        position=Right;
        return position;
    }
    else if (v.GetX()*w.GetX()<0 || w.GetY()*v.GetY()<0)
    {
        position=Behind;
        return position;
    }
    else if (v.ComputeNorm2()<w.ComputeNorm2())
    {
        position =Forward;
        return position;
    }
    else if (w.ComputeNorm2()<=tolerance && w.ComputeNorm2()>= - tolerance)
    {
        position = Begin;
        return position;
    }
    else if (k.ComputeNorm2()<=tolerance && k.ComputeNorm2()>= - tolerance)
    {
        position=End;
        return position;
    }
    else
    {
        position =In;
        return position;
    }
}

bool Polygon::PuntoInterno(const Point &punto)
{
    bool interno=true;
    PositionSide posizione;
    int n=_polygon.size();
    if (TypePolygon()==false)
        for (int i=0; i<n; i++)
        {
            posizione=SidePoint(GetEdge(i), punto);
            if (posizione==PositionSide::Right || posizione==PositionSide::Behind|| posizione==PositionSide::Forward || posizione==PositionSide::Begin || posizione== PositionSide::End)
                interno=false;
        }
    return interno;
}

Polygon &Polygon::operator =(const Polygon &polygon)
{
    int n=polygon._polygon.size();
    _polygon.resize(n);
    for (int i=0; i<n; i++)
    {
        _polygon[i]=polygon._polygon[i];
    }
    return *this;
}

};
