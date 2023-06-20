#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void s21_strcpy(char* child_str, char* parent_str) {
  while (*child_str++ = *parent_str++)
    ;
}

int main() {
  char parent_str[] = "string";
  char* child_str = (char*)malloc(7 * sizeof(char));
  char* another_child_str = (char*)calloc(7, sizeof(char));

  // int count = 0;
  // while (*(child_str + count) = *(parent_str + count)) count++;
  // printf("%d\n", count);

  s21_strcpy(child_str, parent_str);
  strcpy(another_child_str, parent_str);
  printf("parent: %s\nchild: %s\nanother child: %s", parent_str, child_str,
         another_child_str);

  // for (int i = 0; i < count; i++) {;}
}
