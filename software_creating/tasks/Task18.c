#include <stdio.h>
#include <stdlib.h>

// В матрице MxN найти строку с наименьшей суммой элементов и вывести
// индекс. Ввод размерности и элементов матрицы с клавиатуры

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
    int idx = 0, min = 0, sum = 0;
    for (int j = 0; j < N; j++)
        min += arr[0][j];
    for (int i = 1; i < M; i++) {
        sum = 0;
        for (int j = 0; j < N; j++)
            sum += arr[i][j];
        if (sum < min) {
            min = sum;
            idx = i;
        }
    }
    printf("index = %d", idx);
    return 0;
}
