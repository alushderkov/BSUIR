#include <stdio.h>
#include <stdlib.h>

// Дано N и массив целых чисел размерности N, найти минимальный
// четный элемент, а если их нет то вывести "no"

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int *arr = (int *) calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        arr[i] = i % 5 + i ^ 3;
        printf("%2d ", arr[i]);
    }
    printf("\n");
    int min = -1, j = 0;
    while (j < N && min == -1) {
        if (!(arr[j] & 1))
            min = arr[j];
        j++;
    }
    for (int i = j; i < N; i++)
        if (!(arr[i] & 1) && arr[i] < min)
            min = arr[i];
    if (min == -1)
        printf("no");
    else
        printf("%d", min);
    return 0;
}
