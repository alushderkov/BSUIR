#include <stdio.h>

// Перевод в n-ую систему счисления

int main() {
    int num, n;
    printf("num = ");
    scanf("%d", &num);

    printf("n = ");
    scanf("%d", &n);

    int res = 0, digit, power = 1;
    while (num > 0) {
        digit = num % n;
        num /= n;
        res += digit * power;
        power *= 10;
    }

    printf("res = %d", res);
    return 0;
}