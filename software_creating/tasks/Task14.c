#include <stdio.h>

// Проверить, является ли строка выражением
// (состоит из целых цифр, знаков операций и пробелов)

typedef enum {
    digit,
    operator,
    space
} sign;

const int state[3][3] = {
        {0, -1, 1},
        {-1, 2, 1},
        {0, -1, 2}
};

int conv(char c) {
    if (c >= '0' && c <= '9')
        return digit;
    else if (c == '+' || c == '-' || c == '*' || c == '/')
        return operator;
    else if (c == ' ')
        return space;
    return -1;
}

int main() {
    char src[100];
    gets(src);
    int i = 0, curr = 0;
    while (src[i] && curr != -1)
        curr = state[curr][conv(src[i++])];
    printf(curr ? "no" : "yes");
    return 0;
}