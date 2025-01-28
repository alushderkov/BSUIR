#include <stdio.h>

// Найти все вхождения символа в строку и сформировать новую
// из тех, которая без этого символа

int main() {
    char startString[100];
    char finishString[100];

    printf("Enter the string: \n");
    gets(startString);

    printf("Enter the symbol: \n");
    char symbol = (char) getchar();

    int i = 0, j = 0;
    while (startString[i]) {
        if (startString[i] != symbol) {
            finishString[j] = startString[i];
            j++;
        }
        i++;
    }
    finishString[j] = '\0';

    printf("The result string is: \n");
    puts(finishString);

    return 0;
}
