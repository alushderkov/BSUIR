using Microsoft.Win32;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;

namespace Lab1WPF
{
    public class MetricStats
    {
        public int TotalOperators { get; set; } // Количество всех операторов
        public int CL { get; set; }  // Сложность по Джилбу (Cyclomatic Complexity)
        public double Cl { get; set; }  // Сложность (общее количество операторов)
        public int CLI { get; set; }  // Индекс глубины циклической сложности
    }

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void LoadFile_Click(object sender, RoutedEventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "All Files (*.*)|*.*",
                FilterIndex = 1,
                RestoreDirectory = true
            };

            if (openFileDialog.ShowDialog() == true)
            {
                var fullPath = openFileDialog.FileName;
                var sourceCode = File.ReadAllText(fullPath);

                var parser = new Parser2(sourceCode);
                parser.Parse();

                var metricStats = new List<MetricStats>
                {
                    new MetricStats
                    {
                        TotalOperators = parser.TotalOperators,
                        CL = parser.CL,  
                        Cl = parser.cl,  
                        CLI = parser.CLI 
                    }
                };
                StatsDataGrid.ItemsSource = metricStats;

                var rows = new List<TableRow>();
                var operands = parser.Operands;
                var operators = parser.Operators;

                int maxCount = Math.Max(operators.Count, operands.Count);
                for (int i = 0; i < maxCount; i++)
                {
                    string operatorValue = i < operators.Count ? operators.ElementAt(i).Key : "";
                    int f1j = i < operators.Count ? operators.ElementAt(i).Value : 0;
                    int f2i = i < operands.Count ? operands.ElementAt(i).Value : 0;
                    string operandValue = i < operands.Count ? operands.ElementAt(i).Key : "";

                    rows.Add(new TableRow
                    {
                        J = i + 1,
                        Operator = operatorValue,
                        F1j = f1j,
                        I = i + 1,
                        Operand = operandValue,
                        F2i = f2i
                    });
                }

                DataGrid.ItemsSource = rows;
            }
        }
    }

    public class TableRow
    {
        public int J { get; set; }
        public string Operator { get; set; }
        public int F1j { get; set; }
        public int I { get; set; }
        public string Operand { get; set; }
        public int F2i { get; set; }
    }
}
