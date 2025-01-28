#include <stdio.h>
#include <malloc.h>

typedef enum {
    letter,
    open,
    close,
    cav
} sign;

const int state[3][4] = {
        {0, 2, 0, 1},
        {1, 1, 1, 0},
        {2, 2, 0, 2}
};

int conv(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return letter;
    else if (c == '{')
        return open;
    else if (c == '}')
        return close;
    else if (c == '"')
        return cav;
    return -1;
}

int main() {
    char *src = (char *) malloc(100 * sizeof(char));
    gets(src);
    char *dest = (char *) malloc(100 * sizeof(dest));
    int i = 0, j = 0, prev, curr = 0;
    while (src[i]) {
        prev = curr;
        curr = state[curr][conv(src[i])];
        if (curr != 2 && prev != 2) {
            dest[j] = src[i];
            j++;
        }
        i++;
    }
    dest[j] = '\0';
    if (curr == 0)
        printf("%s", dest);
    else
        printf("wrong");
    return 0;
}