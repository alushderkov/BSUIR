#include <stdio.h>
#include <stdlib.h>

// Ввод чисел N и K, K < N. Создать массив из N элементов (элементы начинаются с 1-го)
// удалить из массива K-ый элемент. Вывести преобразованный массив, отводя
// под каждый элемент 7 экранных позиций

int main() {
    int N, K;

    printf("Enter N: ");
    scanf("%d", &N);

    printf("Enter K: ");
    scanf("%d", &K);

    float *arr = (float *) calloc(N, sizeof(float));

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
        printf("%7f\n", arr[i]);
    }
    printf("\n");
    for (int i = K - 1; i < N - 1; i++)
        arr[i] = arr[i + 1];

    arr = (float *) realloc(arr, (N - 1) * sizeof(float));

    for (int i = 0; i < N - 1; i++)
        printf("%7f\n", arr[i]);

    free(arr);
    return 0;
}
