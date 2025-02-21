using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;

namespace DMMaA_LABS_1_2
{
    public class MaxMin : AlgorithmBase
    {
        private readonly Random _random = new Random();

        public MaxMin(IEnumerable<Point> points)
        {
            Points = new List<Point>(points);
            var firstCenter = Points[_random.Next(Points.Count)];
            Classes = new List<ClassDots> {new ClassDots(firstCenter)};
        }

        public List<ClassDots> GetClasses()
        {
            Point? newCenter;
            do
            {
                ClearClasses();
                AddPointsToClass();
                newCenter = GetNewCenter();
                AddCenter(newCenter);
            } while (newCenter != null);

            return Classes;
        }

        private void AddCenter(Point? newCenter)
        {
            if (newCenter != null)
            {
                Classes.Add(new ClassDots(newCenter.Value));
            }
        }


        private Point? GetNewCenter()
        {
            var averageCenterDistance = GetAverageCenterDistance();
            var newCenterCandidate = GetMaxPoint(GetClassesMaxPoints());
            if (newCenterCandidate.PointDistance > averageCenterDistance / 2)
            {
                return newCenterCandidate.MaxPoint;
            }

            return null;
        }

        private double GetAverageCenterDistance()
        {
            var distanceSum = 0.0;
            for (var i = 0; i < Classes.Count; i++)
            {
                for (var j = i + 1; j < Classes.Count; j++)
                {
                    distanceSum += GetPointsInstance(Classes[i].Center, Classes[j].Center);
                }
            }

            var count = Enumerable.Range(1, Classes.Count - 1).Sum();
            return count == 0 ? 0 : distanceSum / count;
        }


        private static ClassMaxDot GetMaxPoint(IEnumerable<ClassMaxDot> points)
        {
            var classMaxDot = new ClassMaxDot {PointDistance = 0};
            foreach (var point in points)
            {
                if (point.PointDistance > classMaxDot.PointDistance)
                {
                    classMaxDot = point;
                }
            }

            return classMaxDot;
        }

        private IEnumerable<ClassMaxDot> GetClassesMaxPoints()
        {
            return Classes.Select(GetClassMaxPoint);
        }

        private ClassMaxDot GetClassMaxPoint(ClassDots pointClass)
        {
            var maxPoint = new ClassMaxDot {PointDistance = 0};
            foreach (var point in pointClass.Points)
            {
                var pointDistance = GetPointsInstance(point, pointClass.Center);
                if (pointDistance > maxPoint.PointDistance)
                {
                    maxPoint = new ClassMaxDot
                    {
                        PointDistance = pointDistance,
                        MaxPoint = point
                    };
                }
            }

            return maxPoint;
        }

        private class ClassMaxDot
        {
            public double PointDistance { get; set; }

            public Point MaxPoint { get; set; }
        }
    }
}