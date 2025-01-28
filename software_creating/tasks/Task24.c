#include <stdio.h>

// Задача на дублирование введённого символа в строке
// Было "ababc", символ 'а', итог "aabaabc"

int main() {
    char src[100], dest[200];
    printf("S: ");
    gets(src);
    printf("C: ");
    char c = (char) getchar();
    int i = 0, j = 0;
    while (src[i]) {
        dest[j++] = src[i];
        if (src[i] == c)
            dest[j++] = src[i];
        i++;
    }
    dest[j] = '\0';
    puts(dest);
    return 0;
}