#include <stdio.h>

// Вывести N элементов ряда Фибоначчи
// 0 1 1 2 3 5 8 13 21 ...

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int n1 = 0, n2 = 1, n3;
    if (N == 1)
        printf("%d", n1);
    else if (N > 1) {
        printf("%d %d ", n1, n2);
        for (int i = 3; i <= N; i++) {
            n3 = n1 + n2;
            printf("%d ", n3);
            n1 = n2;
            n2 = n3;
        }
    }
    return 0;
}