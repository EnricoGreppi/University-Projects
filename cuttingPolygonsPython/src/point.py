import numpy as np
import math


class Point:
    def __init__(self):
        self._coordinate = np.array([0.0, 0.0])
    def __eq__(self, other):
        if (isinstance(other, Point)): #verifico se sono della stessa classe, sono due punti
            return self._coordinate[0] == other._coordinate[0] and self._coordinate[1] == other._coordinate[1]  #verifico che i punti hanno le coord uguali, ritorna vero se la prima coordinata è uguale
        return False #vuol dire che other non è un punto se entra qui
    #questa mi permette di comparare due punti

    def setcoordinate(self, x: float, y: float) -> None:
        self._coordinate[0] = x
        self._coordinate[1] = y #variabile

    def setvectorcoordinate(self, vector):
        self.setcoordinate(vector[0], vector[1])

    def getcoordinates(self):
        return self._coordinate

    def getX(self):
        return self._coordinate[0]

    def getY(self):
        return self._coordinate[1]

    def norm2(self):
        x = self.getX()
        y = self.getY()
        return math.sqrt(x * x + y * y)

    def computeNorm2(self, p1):
        x1 = self.getX()
        y1 = self.getY()
        x2 = p1.getX()
        y2 = p1.getY()
        return math.sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)))


