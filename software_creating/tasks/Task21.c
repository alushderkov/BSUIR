#include <stdio.h>

// Определить является ли введённое с клавиатуры число степенью двойки

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    printf(N & (N - 1) ? "no" : "yes");
    return 0;
}