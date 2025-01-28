#include <stdio.h>

// Дана строка S и символ. Удалить все символы C из строки S и вывести преобразованную строку

int main() {
    char src[100];
    gets(src);
    char c = (char) getchar();

    int i = 0, temp;
    while (src[i])
        if (src[i] == c) {
            temp = i;
            while (src[i + 1]) {
                src[i] = src[i + 1];
                i++;
            }
            src[i] = '\0';
            i = temp;
        } else
            i++;
    puts(src);
    return 0;
}