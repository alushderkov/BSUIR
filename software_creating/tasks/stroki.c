#include <stdio.h>

int main() {
    char src[100];
    gets(src);
    char dest[100];

    int i, j = 0, temp, len = 0;
    while (src[len]) len++;
    for (i = len - 1; i >= 0; i--) {
        if (src[i] == ' ' || i == 0) {
            temp = i;
            if (i) i++;
            while (src[i] != ' ' && src[i] != '\0') {
                dest[j] = src[i];
                j++;
                i++;
            }
            dest[j++] = ' ';
            i = temp;
        }
    }
    dest[j] = '\0';
    puts(dest);
    return 0;
}