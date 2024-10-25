from unittest import TestCase
import src.segment as segment
import src.point as point
import random
import src.vertex as vertex


class TestSegment(TestCase):
    def defSegment(self):
        testSegment=segment.Segment()
        point1 = point.Point()  # definisco un punto1
        point1.setcoordinate(random.randint(0, 1000), random.randint(0, 1000))
        point2 = point.Point()  # definisco un punto2
        point2.setcoordinate(random.randint(0, 1000), random.randint(0, 1000))
        testSegment.setFromPoint(point1,point2)
        return testSegment, point1, point2


    def test_set_fromvertex(self):
        testSegment = segment.Segment()
        point1 = point.Point()  # definisco un punto1
        point1.setcoordinate(random.randint(0, 1000), random.randint(0, 1000))
        point2 = point.Point()  # definisco un punto2
        point2.setcoordinate(random.randint(0, 1000), random.randint(0, 1000))
        vertex1 = vertex.Vertex()
        vertex1.setPoint(random.randint(0, 40), point1)
        vertex2 = vertex.Vertex()
        vertex2.setPoint(random.randint(0, 40), point2)
        testSegment.setFromvertex(vertex1, vertex2)
        try:
            self.assertEqual(testSegment.getFrom(),point1)
            self.assertEqual(testSegment.getTo(), point2)
        except Exception as ex:
            self.fail()


    def test_get_from(self):
        testSegment, point1, point2 = self.defSegment()
        try:
            self.assertEqual(testSegment.getFrom(),point1)
        except Exception as ex:
            self.fail()


    def test_get_to(self):
        testSegment, point1, point2 = self.defSegment()
        try:
            self.assertEqual(testSegment.getTo(), point2)
        except Exception as ex:
            self.fail()


    def test_side_point(self):
        testSegment = segment.Segment()
        point1 = point.Point()  # definisco un punto1
        point1.setcoordinate(0,5)
        point2 = point.Point()  # definisco un punto2
        point2.setcoordinate(0,10)
        testSegment.setFromPoint(point1, point2)

        pointLeft = point.Point()
        pointLeft.setcoordinate(-1,6)
        pointRight = point.Point()
        pointRight.setcoordinate(1, 6)
        pointBehind = point.Point()
        pointBehind.setcoordinate(0,4)
        pointForward = point.Point()
        pointForward.setcoordinate(0,11)
        pointBegin = point.Point()
        pointBegin.setcoordinate(0, 5)
        pointEnd = point.Point()
        pointEnd.setcoordinate(0, 10)
        pointIn = point.Point()
        pointIn.setcoordinate(0, 6)
        try:
            self.assertEqual(testSegment.sidePoint(pointLeft), segment.PositionSide.Left)
            self.assertEqual(testSegment.sidePoint(pointRight), segment.PositionSide.Right)
            self.assertEqual(testSegment.sidePoint(pointBehind), segment.PositionSide.Behind)
            self.assertEqual(testSegment.sidePoint(pointForward), segment.PositionSide.Forward)
            self.assertEqual(testSegment.sidePoint(pointBegin), segment.PositionSide.Begin)
            self.assertEqual(testSegment.sidePoint(pointEnd), segment.PositionSide.End)
            self.assertEqual(testSegment.sidePoint(pointIn), segment.PositionSide.In)
        except Exception as ex:
            self.fail()



