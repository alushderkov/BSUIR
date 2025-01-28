#include <stdio.h>

// Найти процентное соотношение в строке символа, введенного с клавиатуры

int main() {
    char string[100];

    printf("Enter the string:\n");
    gets(string);

    printf("symbol = ");
    char symbol = (char) getchar();

    int symbolCount = 0, allSymbolsCount = 0, i = 0;

    while (string[i]) {
	    if (string[i] == symbol)
		    symbolCount++;
	    allSymbolsCount++;
        i++;
    }

    printf("%d %d\n", symbolCount, allSymbolsCount);
    float percents = (float) symbolCount * 100 / (float) allSymbolsCount;
    printf("Percent ratio is %2.1f\n",percents);

    return 0;
}
