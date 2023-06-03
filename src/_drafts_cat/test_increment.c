#include <stdio.h>

int main() {
  int i = 0;
  int j = 0;
  printf("i++ = %d\n", i++);
  printf("++j = %d\n", ++j);
  return 0;
}

/*---Результат---------------
 * zeftyrst@ge-b2 bricks_cat % ./test_increment
 * i++ = 0
 * ++j = 1
 */
