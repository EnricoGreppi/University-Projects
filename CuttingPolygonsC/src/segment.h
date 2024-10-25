#ifndef SEGMENT_H
#define SEGMENT_H
#include "vertex.h"

#include "Eigen"

using namespace VertexNamespace;
using namespace std;

namespace SegmentNamespace {


class Segment
{
private:
    Point _from;
    Point _to;
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
    PositionSide SidePoint (const Point & point) const;

    Segment();
    ~Segment();
    Segment( const  Point &da, const Point& a);
    Segment (const Vertex& da,const Vertex& a);
    Point GetFrom() const;
    Point GetTo() const;
};
}
#endif // SEGMENT_H
