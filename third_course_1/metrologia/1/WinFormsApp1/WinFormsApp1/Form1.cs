using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            InitializeDataGridViews(); 
        }

        private void InitializeDataGridViews()
        {
            // Инициализация DataGridView для операторов
            dataGridViewOperands.ColumnCount = 3;
            dataGridViewOperands.Columns[0].Name = "Type";
            dataGridViewOperands.Columns[1].Name = "Name";
            dataGridViewOperands.Columns[2].Name = "Count";
            dataGridViewOperands.Columns[1].Width = 325;

            // Инициализация DataGridView для операндов
            dataGridViewOperators.ColumnCount = 3;
            dataGridViewOperators.Columns[0].Name = "Type";
            dataGridViewOperators.Columns[1].Name = "Name";
            dataGridViewOperators.Columns[2].Name = "Count";
            dataGridViewOperators.Columns[1].Width = 325;
        }

        private void RunLexer(string inputFilePath, string outputFilePath)
        {
            Process process = new Process();
            process.StartInfo.FileName = "a.exe"; // Имя исполняемого файла
            process.StartInfo.Arguments = ""; 
            process.StartInfo.UseShellExecute = false; 
            process.StartInfo.RedirectStandardInput = true; // Перенаправление ввода
            process.StartInfo.RedirectStandardOutput = true; // Перенаправление вывода
            process.StartInfo.RedirectStandardError = true; // Перенаправление ошибок
            process.StartInfo.CreateNoWindow = true; 
            process.Start();

            // Перенаправление ввода
            using (StreamReader inputFile = new StreamReader(inputFilePath))
            {
                using (StreamWriter standardInput = process.StandardInput)
                {
                    standardInput.Write(inputFile.ReadToEnd());
                }
            }

            // Перенаправление вывода
            using (StreamWriter outputFile = new StreamWriter(outputFilePath))
            {
                using (StreamReader standardOutput = process.StandardOutput)
                {
                    outputFile.Write(standardOutput.ReadToEnd());
                }
            }
            process.WaitForExit();

            // Вывод сообщений об ошибках 
            string errorOutput = process.StandardError.ReadToEnd();
            if (!string.IsNullOrEmpty(errorOutput))
            {
                Console.WriteLine("Errors: " + errorOutput);
            }
            process.Close();
        }

        private void ProcessOutputFile(string outputFile)
        {
            var operators = new Dictionary<string, int>();
            var operands = new Dictionary<string, int>();

            // Чтение файла
            string[] lines = File.ReadAllLines(outputFile);
            foreach (var line in lines)
            {
                if (line.StartsWith("Operator: "))
                {
                    string op = line.Substring(10).Trim();
                    if (operators.ContainsKey(op))
                        operators[op]++;
                    else
                        operators[op] = 1;
                }
                else if (line.StartsWith("Operand: "))
                {
                    string operand = line.Substring(9).Trim();
                    if (operands.ContainsKey(operand))
                        operands[operand]++;
                    else
                        operands[operand] = 1;
                }
            }

            // Очистка таблиц
            dataGridViewOperands.Rows.Clear();
            dataGridViewOperators.Rows.Clear();

            // Вывод
            foreach (var op in operators)
            {
                dataGridViewOperators.Rows.Add("Operator", op.Key, op.Value); 
            }
            foreach (var operand in operands)
            {
                dataGridViewOperands.Rows.Add("Operand", operand.Key, operand.Value);
            }

            // метрики
            int N1 = operators.Values.Sum(); // Общее число операторов 
            int N2 = operands.Values.Sum();  // Общее число операндов 
            int n1 = operators.Count;        // Словарь операторов 
            int n2 = operands.Count;         // Словарь операндов 
            int vocabulary = n1 + n2;        // Словарь программы
            int length = N1 + N2;            // Длина программы
            double volume = length * Math.Log2(vocabulary);  // Объем программы
            txtN1.Text += N1.ToString();
            txtN2.Text += N2.ToString();
            textn1.Text += n1.ToString();
            textn2.Text += n2.ToString();
            txtVocabulary.Text += vocabulary.ToString();
            txtLength.Text += length.ToString();
            txtVolume.Text += volume.ToString("F2");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string inputFilePath = "txtInputFile.txt";
            string outputFilePath = "txtOutputFile.txt";

            if (File.Exists(inputFilePath))
            {
                RunLexer(inputFilePath, outputFilePath);

                if (File.Exists(outputFilePath))
                {
                    ProcessOutputFile(outputFilePath);
                }
                else
                {
                    MessageBox.Show("Файл вывода не найден.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Файл для обработки не найден.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
