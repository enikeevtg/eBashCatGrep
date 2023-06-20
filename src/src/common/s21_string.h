/*
 *  src/common/s21_string.h
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  zeftyrst@student.21-school.ru
 */

#ifndef SRC_COMMON_S21_STRING_H_
#define SRC_COMMON_S21_STRING_H_

#define S21_NULL ((void*)0)
typedef unsigned long int s21_size_t;

int s21_strlen(const char* str);
int s21_strcmp(const char* str1, const char* str2);
void s21_strcpy(char* child_str, char* parent_str);
char* s21_strchr(char* str, char symb);

#endif  // SRC_COMMON_S21_STRING_H_
