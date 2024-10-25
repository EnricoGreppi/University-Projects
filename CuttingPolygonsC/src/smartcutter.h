#ifndef SMARTCUTTER_H
#define SMARTCUTTER_H


#include <iostream>
#include <list>
#include<queue>
#include <unordered_map>
#include <vector>
#include "Eigen"
#include "cutter.h"



using namespace CutterNamespace;
using namespace std;
using namespace Eigen;

namespace smartCutterNamespace {

class SmartCutter
{
public:
    ConvexCutter _convesso;
    ConcaveCutter _concavo;
    SmartCutter();
    SmartCutter(const vector<Point> &points, const vector<Vertex> &polygonVertices, const Segment &segment);
};
}
#endif // SMARTCUTTER_H
