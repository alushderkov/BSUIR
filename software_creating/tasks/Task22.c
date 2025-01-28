#include <stdio.h>

// Ввести строку S, посчитать количество заглавных латинских букв

int main() {
    char s[100];
    gets(s);
    int count = 0, i = 0;
    while (s[i]) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            count++;
        i++;
    }
    printf("%d", count);
    return 0;
}
