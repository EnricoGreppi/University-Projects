from unittest import TestCase
import src.vertex as vertex
import src.point as point
import random


class TestVertex(TestCase):
    def defVertex(self):
        testVertex = vertex.Vertex() #definisco un vertice
        testPoint = point.Point() #definisco un punto
        testPoint.setcoordinate(random.randint(0, 1000), random.randint(0, 1000))
        testVertex.setPoint(random.randint(0,40),testPoint) #assegno il punto e il nome al vertice
        return testVertex, testPoint

    def test_set_vertex(self):
        testVertex = vertex.Vertex()
        vett = [random.randint(0, 40), random.randint(0, 1000), random.randint(0,1000)]   #mi salvo i valori in un vettore
        testVertex.setVertex( vett[0],vett[1], vett[2])
        try:
            self.assertEqual(testVertex.Id, vett[0])
            self.assertEqual(testVertex.getX(), vett[1])
            self.assertEqual(testVertex.getY(), vett[2])
        except Exception as ex:
            self.fail()


    def test_get_point(self):
        testVertex,testPoint = self.defVertex()
        try:
            self.assertEqual(testVertex.getPoint(), testPoint) 
        except Exception as ex:
            self.fail()

    def test_get_x(self):
        testVertex, testPoint = self.defVertex()
        try:
            self.assertEqual(testVertex.getX(),testPoint.getX())
        except Exception as ex:
            self.fail()

    def test_get_y(self):
        testVertex, testPoint = self.defVertex()
        try:
            self.assertEqual(testVertex.getY(), testPoint.getY())
        except Exception as ex:
            self.fail()
