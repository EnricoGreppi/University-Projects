from unittest import TestCase
import src.point as point
import src.segment as segment
import src.intersector as intersector


class TestIntersector(TestCase):

    def test_compute_intersection(self):
        a= point.Point()
        a.setcoordinate(0, 0)
        b = point.Point()
        b.setcoordinate(4, 0)
        c = point.Point()
        c.setcoordinate(1, 0)
        d = point.Point()
        d.setcoordinate(2, 0)
        ab= segment.Segment()
        ab.setFromPoint(a,b)
        cd = segment.Segment()
        cd.setFromPoint(c, d)
        intersectorNew =intersector.Intersector()
        intersectorNew.setCutter(ab)
        intersectorNew.setEdge(cd)
        intersectorNew.computeIntersection()
        try:
            self.assertEqual(intersectorNew.computeIntersection(), True)
            self.assertEqual(intersectorNew.getTypeIntersection(),intersectorNew.getTypeIntersection())
            self.assertEqual(0.25, intersectorNew.getFirstParametricCoordinate())
            self.assertEqual(0.5, intersectorNew.getSecondParametricCoordinate())
        except Exception as ex:
            self.fail()

        a = point.Point()
        a.setcoordinate(1, 0)
        b = point.Point()
        b.setcoordinate(5, 0)
        c = point.Point()
        c.setcoordinate(3, -6)
        d = point.Point()
        d.setcoordinate(3, 6)
        ab = segment.Segment()
        ab.setFromPoint(a, b)
        cd = segment.Segment()
        cd.setFromPoint(c, d)
        intersector2 = intersector.Intersector()
        intersector2.setCutter(ab)
        intersector2.setEdge(cd)
        try:
            self.assertEqual(intersector2.computeIntersection(), True)
            self.assertEqual(intersector2.getTypeIntersection(), intersector.Type.IntersectionOnSegment)
            self.assertEqual(intersector2.positionIntersectionEdge(), intersector.Position.Inner)
            self.assertEqual(0.5000000000000001, intersector2.getFirstParametricCoordinate())
            self.assertEqual(0.5000000000000001, intersector2.getSecondParametricCoordinate())
            self.assertLessEqual(intersector2.getFirstParametricCoordinate(), 1.0)
            self.assertGreaterEqual(intersector2.getFirstParametricCoordinate(), 0.0)

        except Exception as ex:
            self.fail()


        a = point.Point()
        a.setcoordinate(3, 6)
        b = point.Point()
        b.setcoordinate(3, 2)
        c = point.Point()
        c.setcoordinate(5, 0)
        d = point.Point()
        d.setcoordinate(1, 0)
        ab = segment.Segment()
        ab.setFromPoint(a, b)
        cd = segment.Segment()
        cd.setFromPoint(c, d)
        intersector3 = intersector.Intersector()
        intersector3.setCutter(ab)
        intersector3.setEdge(cd)
        try:
            self.assertEqual(intersector3.computeIntersection(), True)
            self.assertEqual(intersector3.getTypeIntersection(), intersector.Type.IntersectionOnLine)
            self.assertEqual(intersector3.getTypeIntersection(), 1)
            self.assertEqual(1.5000000000000002, intersector3.getFirstParametricCoordinate())
            self.assertEqual(0.5000000000000001, intersector3.getSecondParametricCoordinate())
            self.assertGreaterEqual(intersector3.getFirstParametricCoordinate(), 1.0)
        except Exception as ex:
            self.fail()






