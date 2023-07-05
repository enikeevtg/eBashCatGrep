#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------
void e_strcpy(char* child_str, char* parent_str) {
  while ((*child_str++ = *parent_str++)) {
    ;
  }
}

char* e_strchr(char* str, char symb) {
  for (; symb != *str && *str != '\0'; str++) {
    ;
  }
  if (*str == '\0') str = NULL;
  return str;
}

void array_printing(int* array) {
  for (int i = 0; i < 6; i++) printf("%d\t", array[i]);
}
//----------------------------------------------------------

int main() {
  int errcode = 0;
  char* argvi = (char*)calloc(11, sizeof(char));
  e_strcpy(argvi, "-einltvopqr");
  char error_ch = '\0';
  char shopts[] = "einclv";
  int flags_mask[6] = {0};

  argvi++;
  while (*argvi && !errcode) {
    char* opt_pos = e_strchr(shopts, *argvi);
    if (opt_pos == NULL) {
      error_ch = *argvi;
      printf("illegal option: %c\n", error_ch);
      errcode = 1;
    } else {
      printf("%d\t%c\n", (int)(opt_pos - shopts), shopts[opt_pos - shopts]);
      flags_mask[opt_pos - shopts] = 1;
    }
    argvi++;
  }

  printf("\n%d\n", errcode);
  array_printing(flags_mask);

  return 0;
}

/*==============================================================================
                              Склад чернового кода
==============================================================================*/
//  for (int i = 1; i < 11 /*&& errcode == 0*/; i++) {
//     char* p_opt = NULL;
//     p_opt = e_strchr(shopts, argvi[i]);
//     if (p_opt == NULL) {
//       error_ch = argvi[i];
//       printf("illegal option: %c\n", error_ch);
//       errcode = 1;
//     } else {
//       printf("%d\t%c\n", (int)(p_opt - shopts), shopts[p_opt - shopts]);
//       flags_mask[p_opt - shopts] = 1;
//     }
