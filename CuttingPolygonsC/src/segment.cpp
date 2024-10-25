#include "segment.h"
namespace SegmentNamespace {



Segment::PositionSide Segment::SidePoint(const Point &point) const
{
    PositionSide position;
    position=Left;
    Point p2;
    Point p0;
    Point p1;
    p0=_from;
    p1=_to;
    p2=point;
    Point v;
    Point w;
    Point k;
    v=Point(p1.GetX()-p0.GetX(), p1.GetY()-p0.GetY());
    w=Point(p2.GetX()-p0.GetX(), p2.GetY()-p0.GetY());
    k=Point (p2.GetX()-p1.GetX(), p2.GetY()-p1.GetY()); //vedi slides geom comp
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
    else if (w.ComputeNorm2()<=tolerance  && w.ComputeNorm2()>=-tolerance )
    {
        position = Begin;
        return position;
    }
    else if (k.ComputeNorm2()<=tolerance  && k.ComputeNorm2()>=-tolerance )
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

Segment::~Segment()
{
}

Segment::Segment()
{
    _from=Point();
    _to=Point();
}

Segment::Segment(const Point &da, const Point &a)
{
    _from=da;
    _to=a;
}

Segment::Segment(const Vertex &da,const Vertex &a)
{
    _from=da.GetPoint();
    _to=a.GetPoint();
}

Point Segment::GetFrom() const
{
    return _from;
}

Point Segment::GetTo() const
{
    return _to;
}

}
