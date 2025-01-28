#include <stdio.h>

// Есть строка, вводить как хочешь. Надо проверить закодирована ли она
// с помощью UTF-8. Размер символа от 1 до 4 байт. и там табличка с масками.
//
// 1 байт: 0xxxxxxx
// 2 байта: 110xxxxx 10xxxxxx
// 3 байта: 1110xxxx 10xxxxxx 10xxxxxx
// 4 байта: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//
// Где x произвольное

int main() {
    wchar_t s[100] = L"открывать";
    int i = 0, res = 1;
    while (s[i] && res) {
        if ((s[i] & 0xF8) == 0xF0 && (s[i + 1] & 0xC0) == 0x80 && (s[i + 2] & 0xC0) == 0x80 && (s[i + 3] & 0xC0) == 0x80)
            i += 4;
        else if ((s[i] & 0xF0) == 0xE0 && (s[i + 1] & 0xC0) == 0x80 && (s[i + 2] & 0xC0) == 0x80)
            i += 3;
        else if ((s[i] & 0xE0) == 0xC0 && (s[i + 1] & 0xC0) == 0x80)
            i += 2;
        else if ((s[i] & 0x80) == 0)
            i += 1;
        else
            res = 0;
    }
    printf(res ? "yes" : "no");
    return 0;
}
