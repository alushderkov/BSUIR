#include <stdio.h>

// Алгоритм Евклида. НОД

int main() {
    int a, b;
    printf("a = ");
    scanf("%d", &a);
    printf("b = ");
    scanf("%d", &b);
    while (a != 0 && b != 0) {
        if (a > b)
            a %= b;
        else
            b %= a;
    }
    printf("NOD = %d", a + b);
    return 0;
}