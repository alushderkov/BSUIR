using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace Lab1WPF
{
    public class Parser2
    {
        private string source;
        private Dictionary<string, int> _operands = new();
        private Dictionary<string, int> _operators = new();
        public int UniqueOperators { get; private set; }
        public int TotalOperators { get; private set; }
        public int UniqueOperands { get; private set; }
        public int TotalOperands { get; private set; }

        public int ProgramDict {  get; private set; }

        public int ProgramLen { get; private set; }

        public double Capacity { get; private set; }

        public int CL { get; private set; } 
        public double cl { get; private set; } 
        public int CLI { get; private set; }

        public IReadOnlyDictionary<string, int> Operands => _operands.AsReadOnly();
        public IReadOnlyDictionary<string, int> Operators => _operators.AsReadOnly();

        public Parser2(string source)
        {
            this.source = source;
        }

        public void Parse()
        {
            FindOperators();
            //FindOperands();
            CountTotals();
            CalculateJilbMetricsWithStack();
        }

        private void FindOperands()
        {
            FindVariables();
            FindNumberLiterals();
            FindStringLiterals();
        }

        private void FindOperators()
        {
            DoubleArithmetic();
            DoubleEqual();
            Single();
            FindKeyWords();
            CorrectBranching();
        }

        private void CountTotals()
        {
            foreach (var singleOperator in _operators)
            {
                if (singleOperator.Key == "if" || singleOperator.Key == "for" || singleOperator.Key == "while" || singleOperator.Key == "do" || singleOperator.Key == "else if" || singleOperator.Key == "if...else" || singleOperator.Key == "case")
                {
                    CL += singleOperator.Value;
                }
                TotalOperators += singleOperator.Value;
            }
        }

        #region Operands

        private void FindVariables()
        {
            var regex = new Regex(@"\b[a-zA-Z_]\w*\b");
            source = regex.Replace(source, "");
        }

        private void FindNumberLiterals()
        {
            var regex = new Regex(@"\b\d+(?:\.\d+)?\b");
            source = regex.Replace(source, "");
        }


        private void FindStringLiterals()
        {
            var regex = new Regex(@"\"".*?\""");
            source = regex.Replace(source, "");
        }

        #endregion

        #region Operators

        private void Single()
        {
            var regex = new Regex(@"(\+)|(\-)|(\*)|(\/)|(\%)|(\=)|(\>)|(\<)");


            var matches = regex.Matches(source);
            foreach (var match in matches)
            {
                if (_operators.ContainsKey(match.ToString()!))
                    _operators[match.ToString()!] += 1;
                else
                    _operators.Add(match.ToString()!, 1);
            }


        }

        private void DoubleEqual()
        {
            var regex = new Regex(@"(\+\=)|(\-\=)|(\*\=)|(\/\=)|(\%\=)|(\!\=)|(\<\>)|(\>\=)|(\<\=)");
            var matches = regex.Matches(source);
            foreach (var match in matches)
            {
                if (_operators.ContainsKey(match.ToString()!))
                    _operators[match.ToString()!] += 1;
                else
                    _operators.Add(match.ToString()!, 1);
            }

            source = regex.Replace(source, "");
        }

        private void DoubleArithmetic()
        {

            var regex = new Regex(@"(\+\+)|(\-\-)|(\*\*)|(\=\=)");
            var matches = regex.Matches(source);
            foreach (var match in matches)
            {
                if (_operators.ContainsKey(match.ToString()!))
                    _operators[match.ToString()!] += 1;
                else
                    _operators.Add(match.ToString()!, 1);
            }
            source = regex.Replace(source, "");
        }


     
        private void FindKeyWords()
        {
            var regex = new Regex(@"\b(?:and|xor|or|``|@|using|namespace|new|try|if|else|else if|case|for|while|do)\b");
            var matches = regex.Matches(source);
            foreach (Match match in matches)
            {
                if (_operators.ContainsKey(match.ToString()!))
                    _operators[match.ToString()!] += 1;
                else
                    _operators.Add(match.ToString()!, 1);
            }
        }

        private void CorrectBranching()
        {
            if (_operators.ContainsKey("if"))
            {
                if (_operators.ContainsKey("else"))
                {
                    var ifRegex = new Regex(@"if");
                    var ifMatches = ifRegex.Matches(source);
                    var ifCount = ifMatches.Count;
                        
                    var elseRegex = new Regex(@"else");
                    var elseMatches = elseRegex.Matches(source);
                    var elseCount = elseMatches.Count;
                        
                    var justIfMatches = ifCount - elseCount;
                    _operators["if"] = justIfMatches;
                    if (_operators.ContainsKey("else if"))
                        elseCount -= _operators["else if"];
                    _operators.Add("if...else", elseCount);
                    _operators.Remove("else");
                }
            }
        }

        private void CalculateJilbMetricsWithStack()
        {
            Stack<string> operatorStack = new Stack<string>();
            int currentNesting = 0;
            CLI = 0;
            int conditionCount = 0;
            bool inSwitch = false;

            var lines = source.Split(new[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (var line in lines)
            {
                var cleanLine = Regex.Replace(line, @"//.*|/\*.*?\*/", "").Trim();

                
                if (Regex.IsMatch(cleanLine, @"\b(if|else if|for|while|do)\b"))
                {
                    conditionCount++;
           

                    if (operatorStack.Count > 0)
                    {
                        currentNesting++;
                        CLI = Math.Max(CLI, currentNesting);
                    }
                    operatorStack.Push("conditional");
                }

                if (cleanLine.Contains("}"))
                {
                    if (operatorStack.Count > 0)
                    {
                        string lastOperator = operatorStack.Pop();
                        currentNesting--;

                        if (lastOperator == "case")
                        {
                            while (operatorStack.Count > 0 && operatorStack.Peek() == "case")
                            {
                                operatorStack.Pop();
                                currentNesting--;
                            }
                            operatorStack.Pop();
                            inSwitch = false;
                        }
                        else
                        {

                            currentNesting = Math.Max(0, currentNesting - 1);
                        }
                    }
                }

                
                if (cleanLine.Contains("else") && !cleanLine.Contains("else if"))
                {
                    conditionCount++;
                    

                    if (operatorStack.Count > 0)
                    {
                        currentNesting++;
                        CLI = Math.Max(CLI, currentNesting);
                    }
                    operatorStack.Push("conditional");
                }

                
                if (cleanLine.Contains("switch"))
                {
                    inSwitch = true;
                    operatorStack.Push("switch");
                }

                
                if (cleanLine.Contains("case"))
                {
                    if (inSwitch)
                    {
                        conditionCount++;
                        

                        if (operatorStack.Count > 1)
                        {
                            currentNesting++;
                            CLI = Math.Max(CLI, currentNesting);
                        }
                        operatorStack.Push("case");
                    }
                }

                
                
            }

            cl = (double)CL / TotalOperators;
        }

    }



    #endregion
}

