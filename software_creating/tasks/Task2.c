#include <stdio.h>
#include <stdlib.h>

// 2.Есть массив 987654321. Сделать 975312468 грубо говоря

void printArray(const int * array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("%c", '\n');
}

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);

    int *startArray = (int *) malloc(N * sizeof(int));  // Dynamic array
    for (int i = 0; i < N; ++i)
        startArray[i] = N - i;

    int *newArray = (int *) malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i)
        newArray[i] = N - i;

    printArray(startArray, N);
    int odd_idx = 0, even_idx = N - 1;
    for (int i = 0; i < N; ++i)
        if (startArray[i] & 1)
            newArray[odd_idx++] = startArray[i];
        else
            newArray[even_idx--] = startArray[i];

    printArray(newArray, N);

    free(startArray);
    free(newArray);
    return 0;
}
