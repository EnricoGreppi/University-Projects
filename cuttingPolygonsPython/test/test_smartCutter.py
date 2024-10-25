from unittest import TestCase
import numpy as np
import src.point as point
import src.segment as segment
import src.polygon as polygon
import src.vertex as vertex
import src.cutter as cutter
import src.smartcutter as smartcutter


class TestSmartCutter(TestCase):
    def test_smartCutter(self):
        test = []
        vertices=[]
        p=point.Point()
        p.setcoordinate(2,-2)
        v=vertex.Vertex()
        v.setPoint(0,p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(0, -1)
        v = vertex.Vertex()
        v.setPoint(1, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(3,1)
        v = vertex.Vertex()
        v.setPoint(2, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(0,2)
        v = vertex.Vertex()
        v.setPoint(3, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(3,2)
        v = vertex.Vertex()
        v.setPoint(4, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(3,3)
        v = vertex.Vertex()
        v.setPoint(5, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(-1,3)
        v = vertex.Vertex()
        v.setPoint(6, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(-3,1)
        v = vertex.Vertex()
        v.setPoint(7, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(0,0)
        v = vertex.Vertex()
        v.setPoint(8, p)
        vertices.append(v)
        test.append(p)
        p = point.Point()
        p.setcoordinate(-3,-2)
        v = vertex.Vertex()
        v.setPoint(9, p)
        vertices.append(v)
        test.append(p)
        cut = segment.Segment() #costruttore
        p1=point.Point()
        p1.setcoordinate(-4,-4)
        p2=point.Point()
        p2.setcoordinate(4,4)
        cut.setFromPoint(p1,p2)
        #da 32 a 39 la creazione delle variabili si crea quando la usi perchè non serve il tipo
        #con resize li sostituisco con append

        polygon1= []
        polygon1.append(vertices[0])
        polygon1.append(vertices[1])
        polygon1.append(vertices[2])
        p = point.Point()
        p.setcoordinate(1.4999999999999993, 1.5000000000000002)
        v= vertex.Vertex()
        v.setPoint(10,p)
        polygon1.append(v)
        polygon1.append(vertices[8])
        p = point.Point()
        p.setcoordinate(-1.9999999999999998, -2.0)
        v = vertex.Vertex()
        v.setPoint(11, p)
        polygon1.append(v)
        testP1 = polygon.Polygon(polygon1)

        polygon2 = []
        polygon2.append(vertices[3])
        p = point.Point()
        p.setcoordinate(2.0000000000000004, 2.0)
        v = vertex.Vertex()
        v.setPoint(12, p)
        polygon2.append(v)
        polygon2.append(vertices[5])
        polygon2.append(vertices[6])
        polygon2.append(vertices[7])
        polygon2.append(vertices[8])
        p = point.Point()
        p.setcoordinate(1.4999999999999993, 1.5000000000000002)
        v = vertex.Vertex()
        v.setPoint(10, p)
        polygon2.append(v)
        testP2 = polygon.Polygon(polygon2)

        polygon3 = []
        polygon3.append(vertices[4])
        polygon3.append(vertices[5])
        p = point.Point()
        p.setcoordinate(2.0000000000000004, 2.0)
        v = vertex.Vertex()
        v.setPoint(12, p)
        polygon3.append(v)
        testP3 = polygon.Polygon(polygon3)

        polygon4 = []
        polygon4.append(vertices[9])
        p = point.Point()
        p.setcoordinate(-1.9999999999999998, -2.0)
        v = vertex.Vertex()
        v.setPoint(11, p)
        polygon4.append(v)
        polygon4.append(vertices[8])
        testP4 = polygon.Polygon(polygon4)


        prova = smartcutter.SmartCutter(test,vertices,cut)
        try:
            trovato1 = prova._concave.searchPolygon(testP1) # è un bool
            trovato2 = prova._concave.searchPolygon(testP2)
            trovato3 = prova._concave.searchPolygon(testP3)
            trovato4 = prova._concave.searchPolygon(testP4)
            self.assertEqual(trovato1, True)
            self.assertEqual(trovato2, True)
            self.assertEqual(trovato3, True)
            self.assertEqual(trovato4, True)
        except Exception as ex:
               self.fail()

