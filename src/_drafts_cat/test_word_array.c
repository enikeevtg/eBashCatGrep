//  Тестирование инициализации массива строк с многосимвольными флагами
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"

typedef struct {
  char** lopts;
} data_t;

int main() {
  char opt_b[18] = "--number-nonblank";
  char opt_n[9] = "--number";
  char opt_s[16] = "--squeeze-blank";
  char* array[3] = {opt_b, opt_n, opt_s};
  char word_array[3][18] = {"--number-nonblank", "--number", "--squeeze-blank"};
  data_t data = {0};
  data.lopts = (char**)calloc(3, sizeof(char*));
  data.lopts[0] = "--number-nonblank";
  data.lopts[1] = "--number";
  data.lopts[2] = "--squeeze-blank";

  for (int i = 0; i < 3; i++) {
    printf("%s\n", word_array[i]);
    printf("%s\n", array[i]);
    printf("%s\n", data.lopts[i]);
  }
  // Проверка считывания длины массива строк (fail)
  // char* lopts[] = {"--number-nonblank", "--squeeze-blank", "--number"};
  // printf("%d", e_strlen(lopts));

  return 0;
}
