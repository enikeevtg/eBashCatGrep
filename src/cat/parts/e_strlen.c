//  Функция определения длины строки в двух реализациях:
//  первая сделана через цикл while,
//  вторая - через for.
//  Реализация через for не пропускается компилятором
//  с набором флагов, т.к. "statement with no effect"

#include <stdio.h>
#include <string.h>

size_t e_strlen(const char* str) {
    size_t count = 0;
    while (str[count] != '\0')
        count++;
    return count;
}

size_t e_strlen_alter(const char* str) {
    size_t count = 0;
    for (count; str[count] != '\0'; count++);
    return count;
}

void main() {
    char str[1000];
    scanf("%s", str);
    printf("%d %d", e_strlen(str), e_strlen_alter(str));
}
