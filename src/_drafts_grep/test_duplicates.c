#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** array_fill(int argc, char** argv);
void dupl_check(int argc, char** str_array, int i);
void print_lines(char** array, int argc);
void free_mem(char** array, int argc);

int main(int argc, char** argv) {
  char** lines = array_fill(argc, argv);
  print_lines(lines, argc);
  for (int i = 0; i < argc; i++) dupl_check(argc, lines, i);
  print_lines(lines, argc);
  free_mem(lines, argc);
  return 0;
}

char** array_fill(int argc, char** argv) {
  char** array = (char**)calloc(argc, sizeof(char*));
  for (int i = 0; i < argc; i++) {
    int l = strlen(argv[i]);
    array[i] = (char*)calloc(l, sizeof(char));
    strcpy(array[i], argv[i]);
  }
  return array;
}

void print_lines(char** array, int argc) {
  for (int i = 0; i < argc; i++) printf("%s\t", array[i]);
  printf("\n");
}

void free_mem(char** array, int argc) {
  for (int i = 0; i < argc; i++) free(array[i]);
  free(array);
}

/*==============================================================================
                            Duplicate checking function
==============================================================================*/
void dupl_check(int argc, char** str_array, int i) {
  int duplicate = 0;
  for (int j = 1; j < i && !duplicate; j++) {
    while (!str_array[j] && j < i - 1) j++;
    printf("i = %d, j = %d\n", i, j);
    if (str_array[j] && strcmp(str_array[j], str_array[i]) == 0) {
      printf("duplicate detected\n");
      if (i < argc) {
        free(str_array[i]);  // remove from s21_grep.c
        str_array[i] = NULL;
        duplicate = 1;
      } else {
        free(str_array[i]);
        str_array[i] = NULL;
        duplicate = 1;
      }
      print_lines(str_array, argc);
    }
  }
}
