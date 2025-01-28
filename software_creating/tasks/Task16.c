#include <stdio.h>

// Дано два положительных целочисленных значения A и D
// Вывести число A в системе счисления с основанием D (2 <= D <= 9)
// При этом нельзя, чтобы в начале были нули

int main() {
    int A, D;
    printf("A = ");
    scanf("%d", &A);
    printf("D = ");
    scanf("%d", &D);
    int res = 0, digit, power = 1;
    while (A > 0) {
        digit = A % D;
        A /= D;
        res += digit * power;
        power *= 10;
    }
    printf("%d", res);
    return 0;
}