#include <stdio.h>

// Дан массив и число А. Число встречается несколько
// раз в массиве (может и 0, и 1 раз, и более раз).
// Найти самую последнюю позицию числа А в массиве (но индексация с 1).

int main() {
    int N, A;
    printf("N = ");
    scanf("%d", &N);
    int arr[N];
    for (int i = 0; i < N; i++) {
        printf("arr[%d] = ", i);
        scanf("%d", &arr[i]);
    }
    printf("A = ");
    scanf("%d", &A);
    int idx = N - 1;
    while (idx > -1 && arr[idx] != A) { idx--; }
    printf("%d", idx + 1);
    return 0;
}
