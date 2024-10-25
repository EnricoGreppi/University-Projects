import numpy as np
import src.point as point
import src.segment as segment



class Type:
    NoIntersection =0
    IntersectionOnLine=1
    IntersectionOnSegment = 2
    IntersectionParallelOnLine = 3
    IntersectionParallelOnSegment = 4

class Position:
    Begin = 0
    Inner = 1
    End = 2
    Outer = 3

class Intersector:
    def __init__(self):
        self._matrixTangentVector = np.zeros((2,2))

        self._rightHandSide = []
        self._originCutter = point.Point()
        self._TOLERANCEPARALLELISM = 1.0E-7
        self._TOLERANCEINTERSECTION = 1.0E-7
        self._type = Type.NoIntersection #riga 8
        self._resultParametricCoordinates = np.zeros(2) #setto a 0
        self._positionIntersectionCutter = Position.Begin
        self._positionIntersectionEdge = Position.Begin

    def setToleranceParallelism(self, tolerance):
        self._TOLERANCEPARALLELISM = tolerance

    def getToleranceParallelism(self):
        return self._TOLERANCEPARALLELISM

    def setToleranceIntersection(self, tolerance):
        self._TOLERANCEINTERSECTION = tolerance
    def getToleranceIntersection(self):
        return self._TOLERANCEINTERSECTION


    def setCutter(self, firstSegment):
        self._matrixTangentVector[:,0]=firstSegment.getTo().getcoordinates()-firstSegment.getFrom().getcoordinates()
        self._originCutter = firstSegment.getFrom()

    def setEdge(self, secondSegment):
        self._matrixTangentVector[:,1]=secondSegment.getFrom().getcoordinates()-secondSegment.getTo().getcoordinates()
        self._rightHandSide = secondSegment.getFrom().getcoordinates()- self._originCutter.getcoordinates()

    def computeIntersection(self):
        parallelism = np.linalg.det(self._matrixTangentVector)
        self._type = Type.NoIntersection
        intersection = False
        check = pow(np.linalg.norm(self._matrixTangentVector[:,0]),2)\
                *pow(np.linalg.norm((self._matrixTangentVector[:,1])),2)\
                * self._TOLERANCEPARALLELISM * self._TOLERANCEPARALLELISM
        if pow(parallelism,2) > check:
            solverMatrix = [[self._matrixTangentVector[1,1],-self._matrixTangentVector[0,1]], [-self._matrixTangentVector[1,0], self._matrixTangentVector[0,0]]]
            self._resultParametricCoordinates[0] = solverMatrix[0][0] * self._rightHandSide[0]+ solverMatrix[0][1]*self._rightHandSide[1]
            self._resultParametricCoordinates[1] = solverMatrix[1][0] * self._rightHandSide[0] + solverMatrix[1][1] * \
                                                   self._rightHandSide[1]
            self._resultParametricCoordinates /= parallelism
            if self._resultParametricCoordinates[1] > - self._TOLERANCEINTERSECTION\
                and self._resultParametricCoordinates[1]-1.0 < self._TOLERANCEINTERSECTION:
                self._type= Type.IntersectionOnLine
                intersection = True
                if self._resultParametricCoordinates[0] > -self._TOLERANCEINTERSECTION\
                    and self._resultParametricCoordinates[0]-1.0 <self._TOLERANCEINTERSECTION:
                    self._type= Type.IntersectionOnSegment
        else:
            parallelism2 = abs(self._matrixTangentVector[0,0]\
                               * self._rightHandSide[1]- self._rightHandSide[0]\
                               *self._matrixTangentVector[1,0])
            squaredNormFirstEdge = np.linalg.norm(self._matrixTangentVector[:,0])*np.linalg.norm(self._matrixTangentVector[:,0]) #AL QUADRATO
            check2 = self._TOLERANCEPARALLELISM*self._TOLERANCEPARALLELISM* squaredNormFirstEdge* pow(np.linalg.norm(self._rightHandSide),2)
            if(parallelism2*parallelism2 <= check2):
                tempNorm = 1.0/squaredNormFirstEdge
                #self._resultParametricCoordinates[0] = np.dot(self._matrixTangentVector[:,0],self._rightHandSide)*tempNorm #(self._matrixTangentVector[0,0]*self._rightHandSide[0])+(self._matrixTangentVector[1,0]*self._rightHandSide[1])
                self._resultParametricCoordinates[0] = ((self._matrixTangentVector[0, 0] * self._rightHandSide[0]) + (self._matrixTangentVector[1, 0] * self._rightHandSide[1])) * tempNorm
                #self._resultParametricCoordinates[1] = self._resultParametricCoordinates[0] - np.dot(self._matrixTangentVector[:,0],self._matrixTangentVector[:,1]) * tempNorm
                self._resultParametricCoordinates[1] = self._resultParametricCoordinates[0]- ((self._matrixTangentVector[0, 0] * self._matrixTangentVector[0 , 1]) + (self._matrixTangentVector[1, 0]* self._matrixTangentVector[1,1])) * tempNorm

                intersection = True
                self._type= Type.IntersectionParallelOnLine

                if(self._resultParametricCoordinates[1]< self._resultParametricCoordinates[0]):
                    tmp = self._resultParametricCoordinates[0]
                    self._resultParametricCoordinates[0] = self._resultParametricCoordinates[1]
                    self._resultParametricCoordinates[1] = tmp

                a= self._resultParametricCoordinates[0]-1.0
                b= self._resultParametricCoordinates[1]-1.0
                if ((self._resultParametricCoordinates[0] > -self._TOLERANCEINTERSECTION) and (a < self._TOLERANCEINTERSECTION)) or ((self._resultParametricCoordinates[1] > -self._TOLERANCEINTERSECTION) and (b < self._TOLERANCEINTERSECTION)):
                    self._type = Type.IntersectionParallelOnSegment
                else:
                    if (self._resultParametricCoordinates[0] < self._TOLERANCEINTERSECTION) and (self._resultParametricCoordinates[1]-1.0 > -self._TOLERANCEINTERSECTION):
                        self._type = Type.IntersectionParallelOnSegment


        if(self._resultParametricCoordinates[0] < -self._TOLERANCEINTERSECTION or self._resultParametricCoordinates[0] > 1.0 + self._TOLERANCEINTERSECTION):
            self._positionIntersectionCutter = Position.Outer
        elif (self._resultParametricCoordinates[0]> - self._TOLERANCEINTERSECTION and self._resultParametricCoordinates[0] < self._TOLERANCEINTERSECTION):
            self._resultParametricCoordinates[0] = 0.0
            self._positionIntersectionCutter = Position.Begin
        elif (self._resultParametricCoordinates[0] > 1.0 - self._TOLERANCEINTERSECTION and self._resultParametricCoordinates[0] < 1.0 + self._TOLERANCEINTERSECTION):
            self._resultParametricCoordinates[0] = 1.0
            self._positionIntersectionCutter = Position.End
        else:
            self._positionIntersectionCutter = Position.Inner

        if (self._resultParametricCoordinates[1] < -self._TOLERANCEINTERSECTION or self._resultParametricCoordinates[1] > 1.0 + self._TOLERANCEINTERSECTION):
            self._positionIntersectionEdge = Position.Outer
        elif (self._resultParametricCoordinates[1] > - self._TOLERANCEINTERSECTION and self._resultParametricCoordinates[1] < self._TOLERANCEINTERSECTION):
            self._resultParametricCoordinates[1] = 0.0
            self._positionIntersectionEdge = Position.Begin
        elif (self._resultParametricCoordinates[1] > 1.0 - self._TOLERANCEINTERSECTION and self._resultParametricCoordinates[1] <= 1.0 + self._TOLERANCEINTERSECTION):
            self._resultParametricCoordinates[1] = 1.0
            self._positionIntersectionEdge = Position.End
        else:
            self._positionIntersectionEdge = Position.Inner

        return intersection


    def IntersectionCutterParametricCoordinate(self, cutterSegment:segment.Segment):
        return (1-self._resultParametricCoordinates[0])* cutterSegment.getFrom().getcoordinates()+ self._resultParametricCoordinates[0] * cutterSegment.getTo().getcoordinates()

    def IntersectionEdgeParametricCoordinate(self, edgeSegment:segment.Segment):
        return (1-self._resultParametricCoordinates[1])* edgeSegment.getFrom().getcoordinates()+ self._resultParametricCoordinates[1] * edgeSegment.getTo().getcoordinates()

    def getFirstParametricCoordinate(self):
        return self._resultParametricCoordinates[0]

    def getSecondParametricCoordinate(self):
        return self._resultParametricCoordinates[1]

    def getTypeIntersection(self):
        return self._type


    def positionIntersectionEdge(self):
        return self._positionIntersectionEdge

    def positionIntersectionCutter(self):
        return self._positionIntersectionCutter




