#ifndef VERTEX_H
#define VERTEX_H
#include "point.hpp"
#include "Eigen"

using namespace PointNamespace;
using namespace std;

namespace VertexNamespace {
class Vertex
{
private:
    Point _point;
public:
    int Id;

    Vertex();
     ~Vertex();
    Vertex(const int &id, const double &x, const double &y);
    Vertex(const int &id, const Point &point);

    Point GetPoint() const;
    double GetX () const;
    double GetY() const;
    Vertex& operator =(const Vertex& vertex);
    friend inline bool operator==(const Vertex& lhs, const Vertex& rhs){ return (lhs._point==rhs._point && lhs.Id==rhs.Id);}
};
}
#endif // VERTEX_H
