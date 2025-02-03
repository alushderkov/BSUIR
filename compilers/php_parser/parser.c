%option noyywrap

%{

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>

    #define TOKEN_COUNT 500
    #define LINE_COUNT 100

    enum TOKEN {
        Identifier,
        Literal, 
        Equal, 
        Open, 
        Close, 
        Dot, 
        Ddot, 
        Open_php, 
        Close_php, 
        Dir, 
        Open_block, 
        Close_block, 
        Foreach,
        Echo,
        Print, 
        As
    };

    const char* tokenNames[] = {
        "Identifier",
        "Literal", 
        "Equal", 
        "Open", 
        "Close", 
        "Dot", 
        "Ddot", 
        "Open_php", 
        "Close_php", 
        "Dir", 
        "Open_block", 
        "Close_block", 
        "Foreach",
        "Echo",
        "Print", 
        "As"
    };

    struct TOKENS {
        enum TOKEN tokenType;
        int errorLine;
    };
    

    struct TOKENS currentStr[TOKEN_COUNT];
    int nextTokenIndex = 0;
    int tokenIndexCount = 0;
    int lexLine = 1;
    int errorLine;
    bool isError = false;
    bool startSynt = true;

    void printTokens() {
        printf("Tokens in currentStr:\n");
        for (int i = 0; i < tokenIndexCount; i++) {
            struct TOKENS saveTOKEN = currentStr[i];
            printf("Token %d: %s %i\n", i, tokenNames[saveTOKEN.tokenType], saveTOKEN.errorLine);
        }
    }

    bool isPHP();
    bool isOperators(enum TOKEN);
    bool isOperator();
    bool isOp_Input();
    bool isOp_Foreach();
    bool isOp_Equals();
    bool isOp_Echo();
    bool isOp_Print();
    bool isOp_Exist();
    bool isAssos_OR_Ident();
    bool isAssociation();
    bool isParam();
    bool isExpression();
    bool checkTerm(enum TOKEN);

    bool isPHP() {
        return checkTerm(Open_php) && 
               isOperators(Close_php) && 
               checkTerm(Close_php);
    }

    bool isOperators(enum TOKEN stopConditionToken) {
        bool result = true;
        while(result && (currentStr[nextTokenIndex].tokenType != stopConditionToken)) {
            result = isOperator();
        }
        return result;
    }

    bool isOperator() {
        int currentTokenIndex = nextTokenIndex;
        return  (nextTokenIndex = currentTokenIndex, isOp_Input()) || 
                (nextTokenIndex = currentTokenIndex, isOp_Equals()) ||
                (nextTokenIndex = currentTokenIndex, isOp_Foreach());
    }

    bool isOp_Input() {
        int currentTokenIndex = nextTokenIndex;
        return (nextTokenIndex = currentTokenIndex, isOp_Echo()) || 
               (nextTokenIndex = currentTokenIndex, isOp_Print());
    }

    bool isOp_Exist() {
        int currentTokenIndex = nextTokenIndex;
        return   (nextTokenIndex = currentTokenIndex,  checkTerm(Ddot)) ||
                 (nextTokenIndex = currentTokenIndex, isOperators(Close_block));
    }

    bool isOp_Foreach() {
        return checkTerm(Foreach) && 
               checkTerm(Open) && 
               checkTerm(Identifier) &&
               checkTerm(As) &&
               isAssos_OR_Ident() &&
               checkTerm(Close) &&
               checkTerm(Open_block) &&
               isOp_Exist() && 
               checkTerm(Close_block);
    }

    bool isOp_Equals() {
        return checkTerm(Identifier) &&
               checkTerm(Equal) &&
               isParam() &&
               checkTerm(Ddot);
    }

    bool isOp_Echo() {
        return checkTerm(Echo) && isParam() && checkTerm(Ddot);
    }

    bool isOp_Print() {
        return checkTerm(Print) && isParam() && checkTerm(Ddot);
    }

    bool isAssos_OR_Ident() {
        int currentTokenIndex = nextTokenIndex;
        return  (nextTokenIndex = currentTokenIndex, isAssociation()) || 
                (nextTokenIndex = currentTokenIndex, checkTerm(Identifier));
    }

    bool isAssociation() {
        return checkTerm(Identifier) &&
               checkTerm(Dir) &&
               checkTerm(Identifier);
    }

    bool isParam() {
        int currentTokenIndex = nextTokenIndex;
        return  (nextTokenIndex = currentTokenIndex, checkTerm(Identifier)) || 
                (nextTokenIndex = currentTokenIndex, checkTerm(Literal));
    }

    bool checkTerm(enum TOKEN currentToken) {
        bool result;
        enum TOKEN saveToken = currentStr[nextTokenIndex].tokenType;

        /*printf("Checking token: %s (expected: %s) on line %d\n", 
           tokenNames[saveToken], tokenNames[currentToken], currentStr[nextTokenIndex].errorLine);*/

        if (saveToken == currentToken) {
            printf("%s ", tokenNames[saveToken]);
            result = true;
        } else {
            result = false;
            errorLine = currentStr[nextTokenIndex].errorLine;
        }    

        if (nextTokenIndex < tokenIndexCount) nextTokenIndex++;

        return result;
    }


%}

%%

\"([^\"]*)\" {

    printf("Found the token Literal: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Literal;

}

\$[a-zA-Z]+[a-zA-Z0-9_]* {

    printf("Found the token Identifier: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Identifier;

}

"=>" {

    printf("Found the token Dir: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Dir;    

}

= {

    printf("Found the token Equal: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Equal;    

}

[{] {

    printf("Found the token Open_block: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Open_block;    

}

[}] {

    printf("Found the token Close_block: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Close_block;    

}

[(] {

    printf("Found the token Open: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Open;

}

[)] {

    printf("Found the token Close: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Close;

}

\. {

    printf("Found the token Dot: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Dot;

}

; {

    printf("Found the token Ddot: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Ddot;

}

"echo " {

    printf("Found the token Echo: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Echo;    

}

"print " {

    printf("Found the token Print: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Print;    

}

"foreach" {

    printf("Found the token Foreach: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Foreach;    

}

" as " {

    printf("Found the token As: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = As;    

}

"<?php" {

    printf("Found the token Open_php: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Open_php;

}

"?>" {

    printf("Found the token Close_php: %s\n", yytext);
    currentStr[tokenIndexCount].errorLine = lexLine;
    currentStr[tokenIndexCount++].tokenType = Close_php;

}

[ \t] { }

. {

    isError = true;
    startSynt = false;

}

\r?\n { 
    if (isError) {
        printf("\nError was found in the line %d\n\n", lexLine);
    }
    isError = false;
    lexLine++;         
}

%%

int main(void) {

    printf("Lexical analysis stage:\n\n\n");
    yylex();

    printf("\n\n\n\nSyntactic analysis stage:\n\n\n");
    if (startSynt) {
        bool syntCorrect = true;
        while (nextTokenIndex < tokenIndexCount && syntCorrect) {
            syntCorrect = isPHP();
            /*printf("\n %d %d \n", nextTokenIndex, nextTokenIndex);*/
        }
        if (syntCorrect) {
            printf("\n\nSyntax is correct\n\n");
        } else {
            printf("\nError was found in line: %d\n\nSyntax is incorrect\n\n", errorLine);
        }
    } else {
        printf("Errors were found in the lexis analysis - syntax analysis was not started\n\n");
    }

    return 0;
}
