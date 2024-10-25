from unittest import TestCase
import numpy as np
import src.vertex as vertex
import src.polygon as polygon
import src.point as point

class TestPolygon(TestCase):
    def fillPolygonVertices(self):
        vertices = []
        vertices.append(vertex.Vertex())
        vertices[0].setVertex(0,1.0000e+00, 2.0000e+00)
        vertices.append(vertex.Vertex())
        vertices[1].setVertex(1, 7.6489e-01, 1.3236e+00)
        vertices.append(vertex.Vertex())
        vertices[2].setVertex(2, 4.8943e-02, 1.3090e+00 )
        vertices.append(vertex.Vertex())
        vertices[3].setVertex(3, 6.1958e-01, 8.7639e-01 )
        vertices.append(vertex.Vertex())
        vertices[4].setVertex(4, 4.1221e-01, 1.9098e-01 )
        vertices.append(vertex.Vertex())
        vertices[5].setVertex(5,1.0000e+00, 6.0000e-01)
        vertices.append(vertex.Vertex())
        vertices[6].setVertex(6, 1.5878e+00, 1.9098e-01 )
        vertices.append(vertex.Vertex())
        vertices[7].setVertex(7,1.3804e+00, 8.7639e-01 )
        vertices.append(vertex.Vertex())
        vertices[8].setVertex(8, 1.9511e+00, 1.3090e+00 )
        vertices.append(vertex.Vertex())
        vertices[9].setVertex(9, 1.2351e+00, 1.3236e+00 )
        return vertices



    def test_number_vertices(self):
        p= polygon.Polygon(self.fillPolygonVertices())
        try:
            self.assertEqual(p.numberVertices(), 10)
        except Exception as ex:
            self.fail()

    def test_get_vertex(self):
        vertices = self.fillPolygonVertices()
        p = polygon.Polygon(vertices)
        try:
            for i in range(0,p.numberVertices()):
                self.assertEqual(p.getVertex(i), vertices[i]) #no test coda perchè qui è uguale al vettore
        except Exception as ex:
               self.fail()


    def test_get_edge(self):
        vertices = self.fillPolygonVertices()
        p = polygon.Polygon(vertices)
        prova = p.getEdge(2)
        try:
            p1=point.Point()
            p1.setcoordinate(4.8943e-02, 1.3090e+00)
            self.assertEqual(prova.getFrom(), p1) #no test coda perchè qui è uguale al vettore
            p1.setcoordinate(6.1958e-01, 8.7639e-01)
            self.assertEqual(prova.getTo(), p1)
        except Exception as ex:
               self.fail()
        prova2 = p.getEdge(9)
        try:
            p1 = point.Point()
            p1.setcoordinate(1.2351e-00,1.3236+00)
            self.assertEqual(prova2.getFrom(), p1) #no test coda perchè qui è uguale al vettore
            p1.setcoordinate(1.0000e-00, 2.0000e-00)
            self.assertEqual(prova2.getTo(), p1)
        except Exception as ex:
               self.fail()


    def test_convex_polygon(self):
        vertices = []
        vertices.append(vertex.Vertex())
        vertices[0].setVertex(0,0,0)
        vertices.append(vertex.Vertex())
        vertices[1].setVertex(1, 10,0)
        vertices.append(vertex.Vertex())
        vertices[2].setVertex(2, 10,5)
        vertices.append(vertex.Vertex())
        vertices[3].setVertex(3, 0, 5)

        testconvesso = polygon.Polygon(vertices)
        try:
            self.assertEqual(testconvesso.typePolygon(), False)
        except Exception as ex:
            self.fail()

    def test_concave_polygon(self):
        vertices = []
        vertices.append(vertex.Vertex())
        vertices[0].setVertex(0,0,0)
        vertices.append(vertex.Vertex())
        vertices[1].setVertex(1, 10,0)
        vertices.append(vertex.Vertex())
        vertices[2].setVertex(2, 10,5)
        vertices.append(vertex.Vertex())
        vertices[3].setVertex(3, 2, 2)
        vertices.append(vertex.Vertex())
        vertices[4].setVertex(4, 0, 5)

        testconcavo = polygon.Polygon(vertices)
        try:
            self.assertEqual(testconcavo.typePolygon(), True)
        except Exception as ex:
            self.fail()


