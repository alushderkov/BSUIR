#include <stdio.h>

// Дана строка S и символ C. Слова в строке разделены одним пробелом. Найти кол-во слов с символом C.

int main() {
    char s[100];
    printf("S: ");
    gets(s);
    printf("C: ");
    char c = (char) getchar();
    int word_count = 0, char_count = 0;
    int i = 0;
    while (s[i]) {
        if (s[i] == c)
            char_count++;
        if ((s[i] == ' ' || s[i + 1] == '\0') && char_count > 0) {
            word_count++;
            char_count = 0;
        }
        i++;
    }
    printf("%d", word_count);
    return 0;
}