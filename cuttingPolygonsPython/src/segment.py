import numpy as np
import src.point as point
import src.vertex as vertex




class PositionSide:  #funziona come enum, classe con delle variabili
    Left=0
    Right = 1
    Forward = 2
    Behind = 3
    Begin = 4
    End = 5
    In = 6


class Segment:
    TOLERANCE = 1.0E-7
    def __init__(self):
        self._from = point.Point()
        self._to = point.Point()

    def setFromPoint(self, da: point.Point, a: point.Point): #setto dal punto
        self._from = da
        self._to = a

    def setFromvertex(self, da: vertex.Vertex, a: vertex.Vertex): #setto dal vertice
        self._from = da.getPoint()
        self._to = a.getPoint()

    def getFrom(self):
        return self._from

    def getTo(self):
        return self._to

    def sidePoint(self, p: point.Point):  #point nome: tipo variabile
        position= PositionSide.Left #riga 8-9 del .cpp
        p0 = self._from
        p1 = self._to
        p2 = p
        v = point.Point()
        w = point.Point()
        k = point.Point()
        v.setcoordinate(p1.getX()-p0.getX(), p1.getY()-p0.getY())
        w.setcoordinate(p2.getX()-p0.getX(), p2.getY()-p0.getY())
        k.setcoordinate(p2.getX()-p1.getX(), p2.getY()-p1.getY())
        controllo = np.array([v.getcoordinates(), w.getcoordinates()])
        z = np.linalg.det(controllo) #determinante
        if z> self.TOLERANCE:
            position =PositionSide.Left
            return position
        elif z<- self.TOLERANCE:
            position=PositionSide.Right
            return position
        elif v.getX()*w.getX()<0 or w.getY()*v.getY()<0:  #da sistemare
            position = PositionSide.Behind
            return position
        elif v.norm2()<w.norm2():
            position = PositionSide.Forward
            return position
        elif w.norm2() >= -self.TOLERANCE and w.norm2() <= self.TOLERANCE :  #senza param
            position = PositionSide.Begin
            return position
        elif k.norm2() >= -self.TOLERANCE and k.norm2() <= self.TOLERANCE:
            position = PositionSide.End
            return position
        else:
            position = PositionSide.In
            return position





