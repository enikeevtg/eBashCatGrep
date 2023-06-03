#include <stdio.h>
#include <stdlib.h>

void array_filling(int* array) {
  for (int i = 0; i < 11; i++) *(array + i) = i;
}

void array_printing(int* array) {
  for (int i = 0; i < 11; i++) printf("%d\t", array[i]);
}

int main() {
  int* array = (int*)calloc(11, sizeof(int));

  if (array != NULL) {
    printf("Memory allocated\n");
    array_filling(array);
    array_printing(array);
    free(array);
    printf("\nMemory cleaned");
  }
  return 0;
}
