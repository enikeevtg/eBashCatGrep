#include <stdio.h>

#define SHOPTS_NUM 6
#define LOPTS_NUM 0
#define TRUE 1
#define FALSE 0

void print_lopts(char** lopts, int lopts_num) {
  for (int i = 0; i < lopts_num; i++) printf("%s", lopts[i]);
}

int main() {
  char shopts[SHOPTS_NUM] = "eivcl";
  printf("%s\n", shopts);
  char* lopts[] = {};
  print_lopts(lopts, LOPTS_NUM);
  return 0;
}
