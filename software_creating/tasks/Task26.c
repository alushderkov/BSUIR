#include <stdio.h>

// Есть число n и массив размерностью n, проверить элементы массива
// на соответствие одной из двух последовательностей:
// x1<x2>x3<x4>x5 ...
// x1>x2<x3>x4<x5 ...
// Вывести наверн "yes", если соответствует хотя бы одной, и "no", если наоборот,
// сделать ещё считывание элементов массива с консоли (дефолт ввод с консоли по элементику массива)

// https://pastebin.com/umeGHjZT

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int arr[N];
    for (int i = 0; i < N; i++) {
        printf("arr[%d] = ", i);
        scanf("%d", &arr[i]);
    }
    int res1 = 1, res2 = 1;
    for (int i = 0; i < N - 1; i += 2) {
        if (arr[i] >= arr[i + 1])
            res1 = 0;
        if (arr[i] <= arr[i + 1])
            res2 = 0;
    }
    for (int i = 1; i < N - 1; i += 2) {
        if (arr[i] <= arr[i + 1])
            res1 = 0;
        if (arr[i] >= arr[i + 1])
            res2 = 0;
    }
    printf(res1 + res2 ? "yes" : "no");
    return 0;
}