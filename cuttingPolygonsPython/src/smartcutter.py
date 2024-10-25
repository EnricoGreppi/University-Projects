import src.polygon as polygon
import src.cutter as cutter


class SmartCutter:
    def __init__(self, points, polygonVertices, segment):
        type = polygon.Polygon(polygonVertices)
        if (type.typePolygon()):
            self._concave = cutter.ConcaveCutter()
            self._concave.cutPolygon(points, polygonVertices, segment)
        else:
            self._convex = cutter.ConvexCutter()
            self._convex.cutPolygon(points, polygonVertices, segment)




