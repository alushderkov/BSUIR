#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Дана строка, состоящая из идентификаторов и чисел, разделенных пробелами
// Необходимо сформировать строку, состоящую из чисел, разделенных запятыми
// (проверки не надо)

int main() {
    char start[100];
    char finish[100];
    gets(start);
    int i = 0, j = 0;
    while (start[i]) {
        if (start[i] < '0' || start[i] > '9')
            while (start[i] != ' ' && start[i] != '\0') i++;
        else {
            while (start[i] != ' ' && start[i] != '\0') {
                finish[j] = start[i];
                j++;
                i++;
            }
            finish[j] = ',';
            j++;
        }
        i++;
    }
    finish[j - 1] = '\0';
    puts(finish);
    return 0;
}
