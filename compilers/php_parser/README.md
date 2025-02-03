# PHP Syntax Parser

## Project Description
This project is a high-performance syntax parser for basic PHP language constructs. It performs a full analysis of the input code, detects syntax errors, and generates a token stream according to the specified grammar.

## Grammar
The parser operates based on the following formal grammar:

| Construction | Production |
|-------------|-----------|
| **PHP** | `Open_php Operators Close_php` |
| **Operators** | `Operator*` |
| **Operator** | `Op_Input \| Op_Equals \| Op_Foreach` |
| **Op_Input** | `Op_Echo \| Op_Print` |
| **Op_Foreach** | `Foreach Open Identifier As Assoc_OR_Indent Close Open_block Op_Exist Close_block` |
| **Op_Equals** | `Identifier Equal Param Ddot` |
| **Op_Echo** | `Echo Param Ddot` |
| **Op_Print** | `Print Param Ddot` |
| **Assoc_OR_Indent** | `Association \| Identifier` |
| **Association** | `Identifier Dir Identifier` |
| **Op_Exist** | `Operators \| Ddot` |
| **Param** | `Identifier \| Literal` |

## Features
The project includes:
- A highly efficient lexical analyzer (Flex) that parses PHP code and extracts tokens.
- A full-featured syntax analyzer that detects errors and indicates their line numbers.
- Output of all recognized lexemes in sequence.
- If a lexical error occurs, it is displayed immediately, and syntax analysis is not performed.
- If lexical analysis is successful, the parser outputs the processed token stream in a single line, followed by syntax validation results.

## Implementation
### Lexical Analyzer (Flex)
The lexical analyzer recognizes the following PHP elements:
- Variables (`$variable`)
- Operators (`=`, `=>`)
- Blocks (`{}`, `()`)
- Commands `echo` and `print`
- The `foreach` construct
- PHP code boundaries (`<?php ... ?>`)

If an error is found, the line number is displayed, and syntax analysis is not performed.

### Syntax Analyzer
The parser uses a recursive descent method. Each language construct is validated by individual functions.

Example of a valid input code:
```php
<?php
echo "Hello, World";
$var = "test";
foreach ($arr as $key => $value) {
    print $value;
}
?>
```

## Running the Project
1. Install `flex` (if not installed):
   ```sh
   sudo apt install flex
   ```
2. Compilation:
   ```sh
   flex lexer.l
   gcc lex.yy.c -o parser -lfl
   ```
3. Run the parser:
   ```sh
   ./parser < input.php
   ```

## Program Output
- For correct syntax:
  ```
  Found the token Open_php: <?php
  Found the token Echo: echo
  Found the token Literal: "Hello, World"
  Found the token Ddot: ;
  Found the token Identifier: $var
  Found the token Equal: =
  Found the token Literal: "test"
  Found the token Ddot: ;
  Found the token Foreach: foreach
  Found the token Open: (
  Found the token Identifier: $arr
  Found the token As: as
  Found the token Identifier: $key
  Found the token Dir: =>
  Found the token Identifier: $value
  Found the token Close: )
  Found the token Open_block: {
  Found the token Print: print
  Found the token Identifier: $value
  Found the token Ddot: ;
  Found the token Close_block: }
  Found the token Close_php: ?>

  Open_php Echo Literal Ddot Identifier Equal Literal Ddot Foreach Open Identifier As Identifier Dir Identifier Close Open_block Print Identifier Ddot Close_block Close_php 
  
  Syntax is correct
  ```
- For lexical analysis errors:
  ```
  Found the token Identifier: $var
  Found the token Equal: =
  Found the token Literal: "test"
  Error was found in the line X
  Found the token Foreach: foreach
  Found the token Open: (
  Errors were found in the lexical analysis - syntax analysis was not started
  ```
- For syntax analysis errors:
  ```
  Found the token Open_php: <?php
  Found the token Echo: echo
  Found the token Literal: "Hello, World"
  Found the token Identifier: $var
  Found the token Equal: =
  Found the token Ddot: ;
  
  Open_php Echo Literal Identifier Equal Ddot 
  
  Error was found in line: X
  Syntax is incorrect
  ```

## Conclusion
This parser is a powerful tool for PHP code analysis, providing high accuracy in syntax validation and early error detection. With strict validation and a structured output format, it is useful for both automated code processing and debugging. Future development may include support for more complex PHP constructs and integration with other static analysis tools.

This project demonstrates a strong foundation in compiler construction, lexical and syntax analysis, and structured error handling, making it a valuable addition to PHP development workflows.
