#include <stdio.h>

// Вводится два числа M и N и массив размерностью MxN
// Найти индекс столбца, в котором сумма элементов максимальна

int main() {
    int M, N;
    printf("M = ");
    scanf("%d", &M);
    printf("N = ");
    scanf("%d", &N);
    int arr[M][N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("arr[%d][%d] = ", i, j);
            scanf("%d", &arr[i][j]);
        }
    }
    int idx = 0, sum, max = 0;
    for (int i = 0; i < M; i++)
        max += arr[i][0];
    for (int j = 1; j < N; j++) {
        sum = 0;
        for (int i = 0; i < M; i++)
            sum += arr[i][j];
        if (sum >= max) {
            max = sum;
            idx = j;
        }
    }
    printf("%d", idx);
    return 0;
}
