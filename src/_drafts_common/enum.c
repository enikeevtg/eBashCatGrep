#include <stdio.h>

enum {
  one, two, three, four, five
};

int main() {
  int arr[five] = {0};
  arr[two] = 2;
  printf("%d", arr[1]);
  return 0;
}
