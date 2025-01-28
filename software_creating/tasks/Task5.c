#include <stdio.h>

// Дано число N. Вывести степени двойки от первой до N-ой

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int num = 1;
    for (int i = 1; i <= N; i++) {
        num <<= 1;
        printf("%d ", num);
    }
    return 0;
}
