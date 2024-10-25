#include "intersector.h"

namespace IntersectorNamespace {
Intersector::Intersector()
{
  toleranceParallelism = 1.0E-7;
  toleranceIntersection = 1.0E-7;
  type = Intersector::NoIntersection;
  resultParametricCoordinates.setZero(2);
}
Intersector::~Intersector()
{

}

void Intersector::SetToleranceParallelism(const double &_tolerance) { toleranceParallelism = _tolerance; }

void Intersector::SetToleranceIntersection(const double &_tolerance) { toleranceIntersection = _tolerance; }

void Intersector::SetCutter(const Segment &firstSegment)
{
    matrixTangentVector.col(0) = firstSegment.GetTo().getCoordinate()- firstSegment.GetFrom().getCoordinate();
    originCutter = firstSegment.GetFrom();
}

void Intersector::SetEdge(const Segment &secondSegment)
{
    matrixTangentVector.col(1) = secondSegment.GetFrom().getCoordinate()-secondSegment.GetTo().getCoordinate();
    rightHandSide = secondSegment.GetFrom().getCoordinate()-originCutter.getCoordinate();
}

// ***************************************************************************
bool Intersector::ComputeIntersection()
{
    double parallelism = matrixTangentVector.determinant(); //per fare il prodotto vettoriale
    type = NoIntersection;
    bool intersection = false;

    double check = toleranceParallelism * toleranceParallelism * matrixTangentVector.col(0).squaredNorm() *  matrixTangentVector.col(1).squaredNorm();
  if(parallelism * parallelism >= check) // prodotto vettoriale non nullo
  {
    Matrix2d solverMatrix;
    solverMatrix << matrixTangentVector(1,1), -matrixTangentVector(0,1), -matrixTangentVector(1,0), matrixTangentVector(0,0);
    // <li> If the edge and the trace are not parallel look for the intersection with parametric coordinates
    resultParametricCoordinates = solverMatrix * rightHandSide; // risultato è vector2d
    resultParametricCoordinates /= parallelism;
    if (resultParametricCoordinates(1) > -toleranceIntersection  && resultParametricCoordinates(1)-1.0 < toleranceIntersection)
    {
      type = IntersectionOnLine;
      intersection = true;
      if (resultParametricCoordinates(0) > -toleranceIntersection  && resultParametricCoordinates(0)-1.0 < toleranceIntersection)
        type = IntersectionOnSegment;
    }
  }
  else
  {
    double parallelism2 = fabs(matrixTangentVector(0,0) * rightHandSide.y() - rightHandSide.x() * matrixTangentVector(1,0));
    // <li> In case of parallelism check if the segment is the same with parametric coordinates

    double squaredNormFirstEdge = matrixTangentVector.col(0).squaredNorm();
    double check2 = toleranceParallelism * toleranceParallelism * squaredNormFirstEdge * rightHandSide.squaredNorm();
    if( parallelism2 * parallelism2 <= check2 )
    {
      double tempNorm = 1.0/squaredNormFirstEdge;
      // parametric coordinates on the trace of the starting point and end point
      resultParametricCoordinates(0) = matrixTangentVector.col(0).dot(rightHandSide) * tempNorm ;
      // -1 perchè nella matrice c'è il segno negativo nella seconda colonna
      resultParametricCoordinates(1) = resultParametricCoordinates(0) - matrixTangentVector.col(0).dot(matrixTangentVector.col(1)) * tempNorm;

      intersection = true;
      type = IntersectionParallelOnLine;

      if(resultParametricCoordinates(1) < resultParametricCoordinates(0))
      {
        double tmp = resultParametricCoordinates(0);
        resultParametricCoordinates(0) = resultParametricCoordinates(1);
        resultParametricCoordinates(1) = tmp;
      }
      // if one vertex is in the edge there is the intersection
      if( (resultParametricCoordinates(0) > -toleranceIntersection && resultParametricCoordinates(0)-1.0 < toleranceIntersection) ||
          (resultParametricCoordinates(1) > -toleranceIntersection && resultParametricCoordinates(1)-1.0 < toleranceIntersection)   )
        type = IntersectionParallelOnSegment;
      else
      {
        //IL PRIMO SEGMENTO DATO IN INPUT E' CONTENUTO NEL SECONDO
        if( ( resultParametricCoordinates(0) < toleranceIntersection && resultParametricCoordinates(1) - 1.0 > -toleranceIntersection) )
          type = IntersectionParallelOnSegment;
      }
    }
  }
  /// </ul>
  if(resultParametricCoordinates(0) < -toleranceIntersection || resultParametricCoordinates(0) > 1.0 + toleranceIntersection)
    positionIntersectionCutter =  Outer;
  else if((resultParametricCoordinates(0) > -toleranceIntersection) && (resultParametricCoordinates(0) < toleranceIntersection))
  {
    resultParametricCoordinates(0) = 0.0;
    positionIntersectionCutter= Begin;
  }
  else if ((resultParametricCoordinates(0) > 1.0 - toleranceIntersection) && (resultParametricCoordinates(0) < 1.0 + toleranceIntersection))
  {
    resultParametricCoordinates(0) = 1.0;
    positionIntersectionCutter = End;
  }
  else
    positionIntersectionCutter = Inner;

  if(resultParametricCoordinates(1) < -toleranceIntersection || resultParametricCoordinates(1) > 1.0 + toleranceIntersection)
    positionIntersectionEdge =  Outer;
  else if((resultParametricCoordinates(1) > -toleranceIntersection) && (resultParametricCoordinates(1) < toleranceIntersection))
  {
    resultParametricCoordinates(1) = 0.0;
    positionIntersectionEdge = Begin;
  }
  else if ((resultParametricCoordinates(1) > 1.0 - toleranceIntersection) && (resultParametricCoordinates(1) <= 1.0 + toleranceIntersection))
  {
    resultParametricCoordinates(1) = 1.0;
    positionIntersectionEdge = End;
  }
  else
    positionIntersectionEdge = Inner;

  return intersection;
}

Vector2d Intersector::IntersectionCutterParametricCoordinate(const Segment &cutterSegment) const
{
    return (1 - resultParametricCoordinates(0)) * cutterSegment.GetFrom().getCoordinate() + resultParametricCoordinates(0) * cutterSegment.GetTo().getCoordinate();
}

Vector2d Intersector::IntersectionEdgeParametricCoordinate(const Segment &edgeSegment) const
{
    return (1 - resultParametricCoordinates(1)) * edgeSegment.GetFrom().getCoordinate() + resultParametricCoordinates(1) * edgeSegment.GetTo().getCoordinate();
}
}
