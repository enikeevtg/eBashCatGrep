#include <stdio.h>
#include <stdlib.h>

#define NUM_SHOPTS 6
#define NUM_LOPTS 0
#define TRUE 1
#define FALSE 0

int main() {
  char** lopts = (char**)calloc(3, sizeof(char*));
  lopts[0] = (char*)malloc(3 * sizeof(char));
  lopts[1] = (char*)calloc(3, sizeof(char));
  lopts[2] = (char*)calloc(3, sizeof(char));
  lopts[0] =
      "(char*) calloc(3, "
      "sizeof(char))aergdvzdkfhg;kg;argha;ekjrhgd;"
      "kfhgfdkljaergaerghdrgkdfjhgerlkgjhrd;gfhg;erugh;dvnfvnaergnj;nvkjnzd."
      "gbserthyjnhbvcdemrnghudfjkm,";
  lopts[1] = "(char*) calloc(3, sizeof(char))";
  lopts[2] = "(char*) calloc(3, sizeof(char))";
  for (int i = 0; i < 3; i++) printf("%s\n", *(lopts + i));
  return 0;
}
