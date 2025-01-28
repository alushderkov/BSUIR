#include <stdio.h>

// Два числа, по ноду найти НОК по формуле НОК=а*(б/нод)
// Нод считать по алгоритму евклида

int nod(int a, int b) {
    while (a != 0 && b != 0)
        if (a > b)
            a %= b;
        else
            b %= a;
    return a + b;
}

int main() {
    int a, b;
    printf("a = ");
    scanf("%d", &a);
    printf("b = ");
    scanf("%d", &b);
    printf("%d", a * b / nod(a, b));
    return 0;
}