#ifndef POINT_H
#define POINT_H
#include "Eigen"

using namespace std;
using namespace Eigen;
namespace PointNamespace {

  class Point {
      private:
      Vector2d _coordinate;
    public:
      Point();
      ~Point();
      Point(const double &x, const double &y);
      Point (const Vector2d &coord);
      Vector2d getCoordinate () const;
      double GetX() const;
      double GetY() const;
      double ComputeNorm2();
      double ComputeNorm2(const Point & p1);
      Point& operator =(const Point& point);
      friend inline bool operator==(const Point& lhs, const Point& rhs){ return lhs._coordinate==rhs._coordinate;}
  };
}

#endif // POINT_H
