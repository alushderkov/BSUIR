using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;

namespace DMMaA_LABS_1_2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        private const int WindowFrameSize = 40;
        public List<Color> Pallete { get; }

        public MainWindow()
        {
            InitializeComponent();
            Pallete = new List<Color>
            {
                Color.FromRgb(255, 0, 0),
                Color.FromRgb(0, 0, 255),
                Color.FromRgb(0, 255, 255),
                Color.FromRgb(255, 0, 255),
                Color.FromRgb(255, 255, 0),
                Color.FromRgb(0, 255, 0),
                Color.FromRgb(0, 0, 0),
                Color.FromRgb(255, 127, 39),
                Color.FromRgb(169, 129, 196),
                Color.FromRgb(255, 147, 147),
                Color.FromRgb(128, 128, 0),
                Color.FromRgb(0, 128, 255),
                Color.FromRgb(91, 0, 91),
                Color.FromRgb(100, 100, 100),
                Color.FromRgb(128, 64, 0),
                Color.FromRgb(64, 0, 0),
                Color.FromRgb(0, 0, 128),
                Color.FromRgb(33, 84, 95),
                Color.FromRgb(255, 128, 255),
                Color.FromRgb(133, 220, 80)
            };
        }

        private void RadioButtonKAverage_OnClick(object sender, RoutedEventArgs e)
        {
            ClassesSlider.IsEnabled = true;
        }

        private void RadioButtonMaxMin_OnClick(object sender, RoutedEventArgs e)
        {
            BlockClassSlider();
        }

        private void RadioButtonMaxMinAverage_OnClick(object sender, RoutedEventArgs e)
        {
            BlockClassSlider();
        }

        private void BlockClassSlider()
        {
            ClassesSlider.IsEnabled = false;
        }


        private void Draw(IReadOnlyList<ClassDots> result)
        {
            var drawingGroup = new DrawingGroup();
            for (var i = 0; i < result.Count; i++)
            {
                DrawClass(i, drawingGroup, result[i]);
            }


            Canvas.Source = new DrawingImage(drawingGroup);
        }

        private void DrawClass(int i, DrawingGroup drawingGroup, ClassDots kMeansClass)
        {
            var ellipses = new GeometryGroup();

            foreach (var point in kMeansClass.Points)
            {
                var pointSize = point == kMeansClass.Center ? 7 : 1;
                ellipses.Children.Add(new EllipseGeometry(point, pointSize, pointSize));
            }

            var classColor = GetColorFromPalette(i);
            var brush = new SolidColorBrush(classColor);
            var geometryDrawing = new GeometryDrawing(brush, new Pen(brush, 1), ellipses) {Geometry = ellipses};
            drawingGroup.Children.Add(geometryDrawing);
        }

        private Color GetColorFromPalette(int number)
        {
            return Pallete[number];
        }

        private static List<Point> GetRandomPoints(int pointsCount, int height, int width)
        {
            var result = new List<Point>(pointsCount);
            var random = new Random();
            for (var i = 0; i < pointsCount; i++)
            {
                result.Add(new Point(random.Next(width), random.Next(height)));
            }

            return result;
        }

        private void AlgorithmDispatcher()
        {
            IEnumerable<Point> points = GetRandomPoints((int) DotsSlider.Value,
                (int) BorderImage.ActualHeight - WindowFrameSize,
                (int) BorderImage.ActualWidth);
            try
            {
                if (RadioButtonKAverage.IsChecked != null && (bool) RadioButtonKAverage.IsChecked)
                {
                    var kMeans = new KAverage(points, (int) ClassesSlider.Value);
                    var result = kMeans.GetReadyClasses();
                    Draw(result);
                }
                else
                {
                    var maxMin = new MaxMin(points);
                    var result = maxMin.GetClasses();
                    if (RadioButtonMaxMinAverage.IsChecked != null && (bool) RadioButtonMaxMinAverage.IsChecked)
                    {
                        var kMeans = new KAverage(points, result);
                        result = kMeans.GetReadyClasses();
                    }

                    Draw(result);
                }
            }
            catch (InvalidOperationException e)
            {
                MessageBox.Show(e.Message + Environment.NewLine + e.StackTrace);
            }
        }

        private void ButtonProcessing_OnClick(object sender, RoutedEventArgs e)
        {
            AlgorithmDispatcher();
        }
    }
}