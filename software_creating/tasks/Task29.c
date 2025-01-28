#include <stdio.h>

// Вводятся целочисленная переменная N и вещественный массив на N элементов. Если элементы в нем
// расположены строго по возрастанию или строго по убыванию, вывести YES, иначе вывести NO.
// про клаву или тип выделяемой памяти ничего не говорилось, поэтому я все статически объявлял

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    float arr[N];
    for (int i = 0; i < N; i++) {
        printf("arr[%d] = ", i);
        scanf("%f", &arr[i]);
    }
    int res1 = 1, res2 = 1;
    for (int i = 0; i < N - 1; i++) {
        if (arr[i] >= arr[i + 1])
            res1 = 0;
        if (arr[i] <= arr[i + 1])
            res2 = 0;
    }
    printf(res1 + res2 ? "yes" : "no");
    return 0;
}