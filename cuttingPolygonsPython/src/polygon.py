import numpy as np
import src.point as point
import src.vertex as vertex
import src.segment as segment
import src.intersector as intersector


class PositionSide:  #funziona come enum, classe con delle variabili
    Left=0
    Right = 1
    Forward = 2
    Behind = 3
    Begin = 4
    End = 5
    In = 6

class Polygon:
    def __init__(self, vertices):

        self._polygon = np.copy(vertices) #con questo fa solo la copia ma non cambia se cambio vertices

        #Per Python gestitsco sia vettori che code con copy erchè per lui sono uguali

    def numberVertices(self):
        return len(self._polygon) #è come il size di C++

    def getVertex(self, vertexPosition):
        return self._polygon[vertexPosition]

    def getEdge(self, start):
        to = point.Point()

        if start == self.numberVertices()-1:
            to = self._polygon[0].getPoint()
        else:
            to = self._polygon[start+1].getPoint()
        s = segment.Segment()
        s.setFromPoint(self._polygon[start].getPoint(), to)
        return s

    def typePolygon(self):
        intersection = intersector.Intersector()
        a= 0 #convesso
        N= self.numberVertices()
        i = 1
        while i<N and a==0:
            j=(i+2)%N
            while j!= (i-1)%N and a==0:
                s1= segment.Segment()
                from1=self._polygon[i-1].getPoint()
                to1=self._polygon[i].getPoint()
                from2=self._polygon[((j-1)%N+N)%N].getPoint()
                to2=self._polygon[j%N].getPoint()
                s1.setFromPoint(from1,to1)
                s2 = segment.Segment()
                s2.setFromPoint(from2,to2)
                intersection.setCutter(s1)
                intersection.setEdge(s2)
                ris= intersection.computeIntersection()
                if ris==True and (intersection.getTypeIntersection() == intersector.Type.IntersectionOnLine):
                    a=1 #concavo
                j=(j+1)%N
            i=i+1
        if a==1:
            return True #concavo
        else:
            return False #convesso

    def sidePoint(self, segment, pointP):
       position= PositionSide.Left
       p0= segment.getFrom()
       p1 = segment.getTo()
       p2=pointP
       v = point.Point()
       w = point.Point()
       k = point.Point()
       v.setcoordinate(p1.getX() - p0.getX(), p1.getY() - p0.getY())
       w.setcoordinate(p2.getX() - p0.getX(), p2.getY() - p0.getY())
       k.setcoordinate(p2.getX() - p1.getX(), p2.getY() - p1.getY())
       controll = np.array([v.getcoordinates(), w.getcoordinates()])
       z = np.linalg.det(controll)
       if z > segment.TOLERANCE:
           position = PositionSide.Left
           return position
       elif z < - segment.TOLERANCE:
           position = PositionSide.Right
           return position
       elif v.getX() * w.getX() < 0 or w.getY() * v.getY() < 0:
           position = PositionSide.Behind
           return position
       elif v.norm2() < w.norm2():
           position = PositionSide.Forward
           return position
       elif w.norm2() == -segment.TOLERANCE:
           position =PositionSide.Begin
           return position
       elif k.norm2() == -segment.TOLERANCE:
           position =PositionSide.End
           return position
       else:
           position = PositionSide.In
           return position

    def insidePoint(self , point2):
        inside= True

        n = len(self._polygon)
        if self.typePolygon() == False:
            i=0
            while i<n:
                position2 = self.sidePoint(self.getEdge(i),point2)
                if position2 == segment.PositionSide.Right or position2== segment.PositionSide.Behind or position2 ==segment.PositionSide.Forward or position2==  segment.PositionSide.Begin or position2== segment.PositionSide.End:
                    inside=False
                    i=i+1
                else:
                    i=i+1
            return inside








