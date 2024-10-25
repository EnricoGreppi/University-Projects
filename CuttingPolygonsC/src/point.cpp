#include "point.hpp"

namespace PointNamespace {

  Point::Point()
  {
      _coordinate.Zero();
  }
  Point::~Point() { }

  Point::Point(const double &x,const double &y)
  {
      _coordinate(0)=x;
      _coordinate(1)=y;
  }

  Point::Point(const Vector2d &coord)
  {
      _coordinate(0)=coord(0);
      _coordinate(1)=coord(1);
  }

  Vector2d Point::getCoordinate()const
  {
      return _coordinate;
  }

   double Point::GetX()  const
  {
    double x;
     x=_coordinate.x();
     return x;
   }

   double Point::GetY() const
   {
       double y;
       y=_coordinate.y();
       return y;
   }

   double Point::ComputeNorm2()
   {
       double x;
       x=_coordinate.x();
       double y;
       y=_coordinate.y();
       return sqrt(x*x+ y*y);
   }

   double Point::ComputeNorm2(const Point &p1)
   {
       double x1;
       double x2;
       double y1;
       double y2;
       x1=p1.GetX();
       x2=_coordinate.x();
       y1=p1.GetY();
       y2=_coordinate.y();
       return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
   }

   Point &Point::operator =(const Point &point)
   {
       _coordinate(0)=point._coordinate.x();
       _coordinate(1)=point._coordinate.y();
       return *this;
   }

}
