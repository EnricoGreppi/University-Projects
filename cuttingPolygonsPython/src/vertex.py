import src.point as point


class Vertex:
    Id = 0

    def __init__(self):
        self.Id = 0
        self._point = point.Point()

    def setVertex(self, id: int, x: float, y: float):  #creo un metodo perchè non posso avere 2 costruttori, invece di un altro init (costruttore)
        self.Id = id
        self._point.setcoordinate(x, y)

    def setPoint(self, id: int, point: point.Point):
        self.Id=id
        self._point.setcoordinate(point.getX(), point.getY()) #assegno il punto al vertice
        return self #ritorna il vertice stesso

    def getPoint(self):
        return self._point

    def getX(self ):
        return self._point.getX()

    def getY(self):
        return self._point.getY()





