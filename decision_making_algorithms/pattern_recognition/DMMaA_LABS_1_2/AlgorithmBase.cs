using System;
using System.Collections.Generic;
using System.Windows;

namespace DMMaA_LABS_1_2
{
    public abstract class AlgorithmBase
    {
        protected List<Point> Points;

        protected List<ClassDots> Classes;

        protected void ClearClasses()
        {
            foreach (var pointClass in Classes)
            {
                pointClass.Points = new List<Point> {pointClass.Center};
            }
        }

        protected void AddPointsToClass()
        {
            foreach (var point in Points)
            {
                AddPointToClass(point);
            }
        }

        private void AddPointToClass(Point point)
        {
            var minDifferentClass = GetMinDifferentClass(point);
            minDifferentClass?.Points.Add(point);
        }

        private ClassDots GetMinDifferentClass(Point point)
        {
            var minDifferent = double.MaxValue;
            ClassDots minDifferentClass = null;

            foreach (var pointClass in Classes)
            {
                if (point == pointClass.Center) return null;
                var different = GetInstance(pointClass, point);
                if (!(different < minDifferent)) continue;
                minDifferent = different;
                minDifferentClass = pointClass;
            }

            return minDifferentClass;
        }

        private double GetInstance(ClassDots pointClass, Point point)
        {
            return GetPointsInstance(pointClass.Center, point);
        }

        protected double GetPointsInstance(Point point1, Point point2)
        {
            return Math.Sqrt(Math.Pow(point1.X - point2.X, 2) + Math.Pow(point1.Y - point2.Y, 2));
        }
    }
}