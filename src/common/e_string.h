#ifndef e_STRING_H
#define e_STRING_H

int e_strlen(const char* str);
int e_strcmp(const char* str1, const char* str2);
void e_strcpy(char* child_str, char* parent_str);
char* e_strchr(char* str, char symb);

#endif  // e_STRING_H
