using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;

namespace DMMaA_LABS_1_2
{
    public class ClassDots
    {
        public List<Point> Points { get; set; }

        public Point Center { get; set; }

        public ClassDots(Point center)
        {
            Points = new List<Point> {center};
            Center = center;
        }


        public Point GetBestClassCenter()
        {
            var bestCenter = new Point(Points.Average(x => x.X), this.Points.Average(x => x.Y));
            var minDifferent = double.MaxValue;
            var minDifferentPoint = new Point();
            foreach (var centerCandidate in this.Points)
            {
                var different = GetDotsInstance(bestCenter, centerCandidate);
                if (!(different < minDifferent)) continue;
                minDifferent = different;
                minDifferentPoint = centerCandidate;
            }

            return minDifferentPoint;
        }

        private static double GetDotsInstance(Point firstDot, Point secondDot)
        {
            var xDifferent = firstDot.X - secondDot.X;
            var yDifferent = firstDot.Y - secondDot.Y;
            return Math.Sqrt(xDifferent * xDifferent + yDifferent * yDifferent);
        }
    }
}