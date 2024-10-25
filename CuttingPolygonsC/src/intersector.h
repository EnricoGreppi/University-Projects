#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include "Eigen"
#include "segment.h"

using namespace SegmentNamespace;
using namespace Eigen;
using namespace std;

namespace IntersectorNamespace {
class Intersector
{
public:
    enum Type
    {
        NoIntersection = 0,
        IntersectionOnLine = 1,
        IntersectionOnSegment = 2,
        IntersectionParallelOnLine = 3,
        IntersectionParallelOnSegment = 4,
    };
    enum Position
    {
        Begin = 0,
        Inner = 1,
        End = 2,
        Outer = 3
    };
private:

    double toleranceParallelism;
    double toleranceIntersection;

    Type type;
    Position positionIntersectionCutter;
    Position positionIntersectionEdge;

    Vector2d resultParametricCoordinates;
    Point originCutter;
    Vector2d rightHandSide;
    Matrix2d matrixTangentVector;
public:
    Intersector();
    ~Intersector();

    void SetToleranceParallelism(const double& _tolerance);
    void SetToleranceIntersection(const double& _tolerance);

    void SetCutter(const Segment &firstSegment);
    void SetEdge(const Segment &secondSegment);

    //Compute the intersections between the line of the first edge and the second edge and returns the parametric coordinates and the intersection type
    //The first parametric coordinate refers to the first tangentVector and ...
    //@note In case of parallelism both the parametric coordinates refers to the first edge
    bool ComputeIntersection();

    const double& ToleranceIntersection() const {return toleranceIntersection; }
    const double& ToleranceParallelism() const {return toleranceParallelism; }

    const Vector2d& ParametricCoordinates() {return resultParametricCoordinates;}
    const double& FirstParametricCoordinate() {return resultParametricCoordinates(0);}
    const double& SecondParametricCoordinate() {return resultParametricCoordinates(1);}

    const Position& PositionIntersectionInCutter(){return positionIntersectionCutter;}
    const Position& PositionIntersectionEdge(){return positionIntersectionEdge;}
    const Type& TypeIntersection() {return type;}

    Vector2d IntersectionCutterParametricCoordinate(const Segment &cutterSegment) const;

    Vector2d IntersectionEdgeParametricCoordinate(const Segment &edgeSegment) const;
};
}
#endif // INTERSECTOR_H
