/*
 *  src/common/e_string.h
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  enikeev.tg@gmail.com
 */

#ifndef SRC_COMMON_E_STRING_H_
#define SRC_COMMON_E_STRING_H_

#define E_NULL ((void*)0)
typedef unsigned long int e_size_t;

int e_strlen(const char* str);
int e_strcmp(const char* str1, const char* str2);
void e_strcpy(char* child_str, char* parent_str);
char* e_strchr(char* str, char symb);

#endif  // SRC_COMMON_E_STRING_H_
