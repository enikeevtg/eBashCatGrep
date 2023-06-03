/*  
 *  test_optmask.c:
 *  Проверка кодирования односимвольных опций
 *  через побитовую маску. Хотел воспользоваться
 *  при вводе опции -l, но с ним многие опции
 *  работают.
 *
 * test:
 *  stdin: gcc test_optbinmask.c -o test_optbinmask
 *         ./test_optbinmask e i v c d l s u n a
 * stdout: 0b1111110100
 *           eivclnhsfo
 *           eivclns%
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#define OPT_NUM 10

int main(int argc, char** argv) {
  char opts[OPT_NUM + 1] = "eivclnhsfo\0";
  int16_t opts_mask = 0;
  for (int i = 1; i < argc; i++) {
    char* popt = NULL;
    popt = strchr(opts, *argv[i]);
    if (popt != NULL)
      opts_mask |= (int16_t)pow(2, 8 * sizeof(int16_t) - (popt - opts + 1));
  }

  printf("0b");
  for (int i = 0; i < OPT_NUM; i++)
    printf("%c", opts_mask & (0b1000000000000000 >> i) ? '1' : '0');
  printf("\n  %s\n  ", opts);
  for (int i = 0; i < OPT_NUM; i++)
    if (opts_mask & (0b1000000000000000 >> i)) printf("%c", opts[i]);

  return 0;
}
