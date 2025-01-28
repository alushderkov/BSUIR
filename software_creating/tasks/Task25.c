#include <stdio.h>

// Дан массив на N элементов, N - четное.
// Найти максимальную сумму пар элементов (пары по типу 1,2 элементы, 3,4 элементы, 5,6 элементы и т. д.)
// и количество пар с такой же суммой.
// Вывести на экран макс сумму и кол-во пар.
// Ввод пользователем через клавиатуру, правильность данных гарантируется.

int main() {
    int N;
    printf("N = ");
    scanf("%d", &N);
    int arr[N];
    for (int i = 0; i < N; i++) {
        printf("arr[%d] = ", i);
        scanf("%d", &arr[i]);
    }
    int max = arr[0] + arr[1], count = 1;
    for (int i = 2; i < N; i += 2) {
        if (arr[i] + arr[i + 1] == max)
            count++;
        if (arr[i] + arr[i + 1] > max) {
            max = arr[i] + arr[i + 1];
            count = 1;
        };
    }
    printf("Sum = %d, count = %d", max, count);
    return 0;
}