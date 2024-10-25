#include "smartcutter.h"
namespace smartCutterNamespace {


SmartCutter::SmartCutter()
{

}

SmartCutter::SmartCutter(const vector<Point> &points, const vector<Vertex> &polygonVertices, const Segment &segment)
{
    Polygon tipo;
    tipo=Polygon(polygonVertices);
    if (tipo.TypePolygon())
    {
        _concavo=ConcaveCutter();
        _concavo.CutPolygon(points,polygonVertices,segment);
    }
    else
    {
        _convesso=ConvexCutter();
        _convesso.CutPolygon(points,polygonVertices,segment);
    }
}
}
