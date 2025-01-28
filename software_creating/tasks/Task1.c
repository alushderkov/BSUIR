#include <stdio.h>

// 1.Найти в строке, содержащей слова и числа, разделенные пробелами, количество вхождений числа N.

int main() {
    char str[] = "Hello 11 my   2 people 4 dwf 5 you 11    to be me 11 d 3";
    int counter = 0;
    int num = 11; //number to find
    char *cursor = str;
    while (*cursor) {
        while (!(*cursor >= '0' && *cursor <= '9'))
            cursor++;
        int parsed_number = 0;
        while (*cursor >= '0' && *cursor <= '9') {
            parsed_number *= 10;
            parsed_number += *cursor - '0';
            cursor++;
        }
        if (parsed_number == num)
            counter++;
    }
    printf("count is %d\n", counter);
    return 0;
}