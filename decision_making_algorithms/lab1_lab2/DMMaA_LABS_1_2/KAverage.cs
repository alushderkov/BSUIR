using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows;

namespace DMMaA_LABS_1_2
{
    public class KAverage : AlgorithmBase
    {
        private readonly Random _random = new Random();

        private bool _isNeedRecalculate;

        public KAverage(IEnumerable<Point> points, int classesCount)
        {
            Points = new List<Point>(points);
            Classes = GetClassesWithRandomCenters(classesCount);
        }

        public KAverage(IEnumerable<Point> points, IEnumerable<ClassDots> classes)
        {
            Points = new List<Point>(points);
            Classes = new List<ClassDots>(classes);
        }

        private List<ClassDots> GetClassesWithRandomCenters(int classesCount)
        {
            var result = new List<ClassDots>();
            var selectedCenters = new List<Point>();
            for (var i = 0; i < classesCount; i++)
            {
                result.Add(new ClassDots(GetNextRandomCenter(selectedCenters)));
            }

            return result;
        }

        private Point GetNextRandomCenter(ICollection<Point> selectedCenters)
        {
            Point centerCandidate;
            do
            {
                centerCandidate = this.Points[_random.Next(this.Points.Count)];
            } while (selectedCenters.Contains(centerCandidate));

            return centerCandidate;
        }

        public List<ClassDots> GetReadyClasses()
        {
            do
            {
                _isNeedRecalculate = false;
                ClearClasses();
                AddPointsToClass();
                ChangeClassCenters();
            } while (_isNeedRecalculate);

            return Classes;
        }

        private void ChangeClassCenters()
        {
            Parallel.ForEach(Classes, kMeansClass =>
            {
                var bestClassCenter = kMeansClass.GetBestClassCenter();

                if (bestClassCenter == kMeansClass.Center) return;
                kMeansClass.Center = bestClassCenter;
                _isNeedRecalculate = true;
            });
        }
    }
}