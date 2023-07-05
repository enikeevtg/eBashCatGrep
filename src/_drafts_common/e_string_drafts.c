#include <stdio.h>  // NULL

#include "e_string.h"
// // На правах копипасты
// #ifndef NULL
// #include <sys/_types.h> /* __DARWIN_NULL */
// #define NULL  __DARWIN_NULL
// #endif  /* NULL */

/*==============================================================================
                          Функция определения длины строки
===================================================================================*/
// unsigned int e_strlen(const char* str) {
int e_strlen(const char* str) {
  int count = 0;
  while (str[count] != '\0') count++;
  return count;
  // Другие варианты реализации:
  // 1) while (*(str + count)) count++;
  // return count;
}

/*==============================================================================
                            Функция сравнения двух строк
===================================================================================*/
int e_strcmp(const char* str1, const char* str2) {
  int count = 0;
  while (str1[count] == str2[count] && str1[count] != '\0') count++;
  return str1[count] - str2[count];
  // Другие варианты реализации:
  // 1) while(*(str1 + count) && *(str1 + count) == *(str1 + count)) count++;
  // return str1[count] - str2[count];
  // 2) for (; *str1 && *str1 == *str2; str1++, str2++) {;}
  // return *str1 - *str2;
}

/*==============================================================================
            Функция копирования содержимого из одной строки в другую
===================================================================================*/
void e_strcpy(char* child_str, char* parent_str) {
  while ((*child_str++ = *parent_str++)) {
    ;
  }
}

/*==============================================================================
            Функция копирования содержимого из одной строки в другую
===================================================================================*/
char* e_strchr(char* str, char symb) {
  for (; symb != *str && *str != '\0'; str++) {
    ;
  }
  if (*str == '\0') str = NULL;
  return str;

  // Другие варианты реализации:
  // 1) while (symb != *str && *str != '\0') str++;
  // if (*str == '\0') str = NULL;
  // return str;
}
