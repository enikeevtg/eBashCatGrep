//  Функция сравнения двух строк в двух реализациях:
//  первая сделана через цикл while,
//  вторая - через for.
//  Реализация через for не пропускается компилятором
//  с набором флагов, т.к. "statement with no effect"

#include <stdio.h>

int strcmp(const char* str1, const char* str2) {
    int count = 0;
    while (str1[count] == str2[count] && str1[count] != '\0')
        count++;
    return str1[count] - str2[count];
}

int strcmp_alter(const char* str1, const char* str2) {
    int count = 0;
    for (count; str1[count] == str2[count] && str1[count] != '\0'; count++);
    return str1[count] - str2[count];
}

void main() {
    char str1[1000], str2[1000];
    scanf("%s", str1);
    scanf("%s", str2);
    printf("%d", !strcmp(str1, str2));
}



/*========================================
            Склад чернового кода
========================================*/
// #include <stdbool.h>
// 
// size_t strlen(const char* str) {
//     size_t count = 0;
//     for(count; str[count] != '\0'; count++);
//     return count;
// }
// bool strcmp(const char* str1, const char* str2) {
//     int count = 0;
//     for(count; str1[count] == str2[count] && str1[count] != '\0'; count++);
//     return (count == strlen(str1)) && (count == strlen(str2));
// }
