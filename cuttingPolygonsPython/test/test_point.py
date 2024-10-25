import unittest
import numpy as np
import src.point as point
import random
import math


class TestPoint(unittest.TestCase):

    def defPoint(self):  #questa è una funzione che crea un punto casuale
        testPoint = point.Point()
        coordinate = [random.randint(0, 1000), random.randint(0, 1000)]
        testPoint.setvectorcoordinate(coordinate)
        return testPoint, coordinate

    def test_getX(self):
        testPoint, coordinate = self.defPoint()
        try:
            self.assertEqual(testPoint.getX(), coordinate[0])
        except Exception as ex:
            self.fail()

    def test_getY(self):
        testPoint, coordinate = self.defPoint()
        try:
            self.assertEqual(testPoint.getY(), coordinate[1])
        except Exception as ex:
            self.fail()

    def test_Norma(self):
         testPoint, coordinate = self.defPoint()
         testNorma = math.sqrt(math.pow(testPoint.getX(),2)+ math.pow(testPoint.getY(),2))
         try:
           self.assertEqual(testPoint.norm2(),testNorma)
         except Exception as ex:
           self.fail()

    def test_getcoordinates(self):
        testPoint,coordinate =self.defPoint()
        try:
          self.assertEqual(testPoint.getcoordinates()[0], coordinate[0])
          self.assertEqual(testPoint.getcoordinates()[1], coordinate[1])
        except Exception as ex:
          self.fail()

    def test_setcoordinate(self):
        testPoint = point.Point()
        coordinate = [random.randint(0, 1000), random.randint(0, 1000)]
        testPoint.setcoordinate(coordinate[0], coordinate[1])
        try:
            self.assertEqual(testPoint.getcoordinates()[0], coordinate[0])
            self.assertEqual(testPoint.getcoordinates()[1], coordinate[1])
        except Exception as ex:
            self.fail()

    def test_ComputeNorm(self):
        testPoint, coordinate = self.defPoint()
        testPoint2, coordinate2 = self.defPoint()
        x2= coordinate2[0]
        x1= coordinate[0]
        y2= coordinate2[1]
        y1= coordinate[1]
        testNorma = math.sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)))
        try:
            self.assertEqual(testPoint.computeNorm2(testPoint2), testNorma)
        except Exception as ex:
            self.fail()
