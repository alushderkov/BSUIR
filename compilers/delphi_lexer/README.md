# Lexical Analyzer for Delphi Language

## Description
This project is a powerful tool for analyzing the syntactic constructs of the Delphi language. It is designed to identify valid constructions and extract correct substrings from the input.

## Features
- Verification of whether the input string conforms to a valid Delphi construct.
- Input processing from a file or keyboard.
- Generation of a list of valid substrings.
- Saving the number of found substrings to the `output.txt` file.
- Analyzing the input string for valid constructions, even if the entire string is incorrect.
- Splitting the string into substrings and checking each one against syntax rules.
- Optimized finite-state machine algorithm for fast and accurate input processing.

## Implementation
The program is developed using C++ and is based on formal language methods and automata theory. Key components include:
- **Finite-state machine** with an optimized transition system.
- **Flexible input processing** (from a file or keyboard) with the ability to handle large amounts of data.
- **Detection of valid substrings**, even if the original input string is incorrect.
- **Optimized memory management**, eliminating redundant computations.

## Usage
1. Run the program.
2. Choose an input method:
   - `0` — Enter a string manually.
   - `1` — Read strings from a file.
3. Enter a string for analysis or specify a file path.
4. Receive the verification result and a list of detected substrings.
5. The number of valid substrings will be saved in `output.txt`.

## Requirements
- C++ compiler (support for C++11 or higher)
- Input file (if file-based input is used)

## Execution
Compilation and execution:
```sh
 g++ lexAnalyzer.cpp -o lexAnalyzer
 ./lexAnalyzer
```

## Example
**Input:**
```text
'Hello''World'
```
**Output:**
```text
The input string is a valid construct in the Delphi language:
'Hello''World'
Valid substrings:
'Hello'
'World'
```

## Contact
This project is designed with a focus on high performance and precise analysis. For questions and suggestions, please use GitHub Issues.
