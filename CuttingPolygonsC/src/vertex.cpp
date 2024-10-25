#include "vertex.h"
namespace VertexNamespace
{
Vertex::Vertex()
{
    Id=0;
    _point=Point();
}

Vertex::~Vertex()
{

}

Vertex::Vertex(const int &id, const double &x, const double &y)
{
   Id=id;
   _point=Point(x,y);
}

Vertex::Vertex(const int &id, const Point & point)
{
    Id=id;
    _point = point;
}

Point Vertex::GetPoint() const
{
    return _point;
}



double Vertex::GetX() const
{
    return _point.GetX();
}

double Vertex::GetY()  const
{
    return _point.GetY();
}

Vertex &Vertex::operator =(const Vertex &vertex)
{
    Id=vertex.Id;
    _point=vertex._point;
    return *this;
}

};
