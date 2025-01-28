#include <stdio.h>

// Вводится число N и массив размером N с клавы, вывести "+",
// если положительных чисел больше, чем отрицательных, "-",
// если наоборот, "=", если одинаковое количество

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int arr[N], pos = 0, neg = 0;
    for (int i = 0; i < N; i++) {
        printf("arr[%d] = ", i);
        scanf("%d", &arr[i]);
        if (arr[i] > 0)
            pos++;
        if (arr[i] < 0)
            neg++;
    }
    if (pos > neg)
        printf("+");
    else if (pos < neg)
        printf("-");
    else
        printf("=");
    return 0;
}
