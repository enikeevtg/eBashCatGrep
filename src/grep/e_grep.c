/*  e_grep
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - too few arguments
 *  2 - illegsl option
 *  3 - no such file or directory
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"

#define OPT_NUM 11  // Количество односимвольных опций

#define TRUE 1
#define FALSE 0

typedef struct {
  char* opt_str;
  bool* opt_mask;
  bool pat_exist;  // pattern input flag
  char** patterns;
  char** files;
} input;

//---ПРОТОТИПЫ_ФУНКЦИЙ-------------------------------------------------------------
void e_grep(int argc, char** argv);
void opt_pat_def(char** argv, int index, input* data);
void opt_print(input data);
void array_print(char** array, int argc);
//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  if (argc > 2)
    e_grep(argc, argv);
  else
    fprintf(stderr, "e_grep: Too few arguments\n");  // ОБРАБОТКА ОШИБКИ 1
  return 0;
}

/*===================================================================================
                              Реализация команды grep
===================================================================================*/
void e_grep(int argc, char** argv) {
  input data = {0};  // input is type of struct variable
  data.opt_str = "eivclnhsfo";
  printf("%c\n", data.opt_str[8]);
  data.pat_exist = FALSE;
  data.patterns = (char**)calloc(argc, sizeof(char*));
  data.files = (char**)calloc(argc, sizeof(char*));

  // parsing
  for (int i = 1; i < argc; i++) {
    // printf("%d\n", i);
    // OPTION and PATTERN after -e:
    if (**(argv + i) == '-') {  // or if (argv[i][0] == '-') {
      opt_pat_def(argv, i, &data);
      // printf("option\n");
    }
    // PATTERN:
    if (**(argv + i) != '-' && !data.pat_exist) {
      *(data.patterns + i) = *(argv + i);
      data.pat_exist = TRUE;
      // printf("pattern\n");
    }
    // FILE:
    if ((**(argv + i)) != '-' && (*(data.patterns + i) == 0)) {
      *(data.files + i) = *(argv + i);
      // printf("file\n");
    }
  }

  opt_print(data);
  printf("\tpatterns: ");
  array_print(data.patterns, argc);
  printf("\n\tfiles: ");
  array_print(data.files, argc);
}


/*===================================================================================
      Функция парсинга опций, паттернов и файлов из аргументов командной строки:
          Реализация через массив (строку), в который помещены все флаги
===================================================================================*/
void opt_pat_def(char** argv, int index, input* data) {
  int i = 1;
  while (argv[index][i] && !data->patterns[index]) {
  // or while ( *(*(argv + index) + i) && !*(data->patterns + index)) {
    char* psymb = *(argv + index) + i;
    
    // opt_e analysis
    if (*psymb == 'e') {
      data->opt_e = 1;
      if (*(psymb + 1) != '\0') {
        data->patterns[index] = psymb + 1;
        data->pat_exist = TRUE;
      } else {
        data->patterns[index + 1] = argv[index + 1];
        data->pat_exist = TRUE;
      }
    }  // if (*psymb == 'e')

    i++;
  }
}

void opt_print(input data) {
  printf(
      "\topt_e: %d\n\topt_i: %d\n\topt_v: %d\n\topt_c: %d\n\topt_l: "
      "%d\n\topt_n: %d\n\topt_h: %d\n\topt_s: %d\n\topt_f: %d\n\topt_o: %d\n",
      data.opt_e, data.opt_i, data.opt_v, data.opt_c, data.opt_l, data.opt_n,
      data.opt_h, data.opt_s, data.opt_f, data.opt_o);
}

void array_print(char** array, int argc) {
  for (int i = 0; i < argc; i++)
    if (array[i] != NULL)
    printf("%s\t", array[i]);
}







/*===================================================================================
                              Склад чернового кода
===================================================================================*/


/*===================================================================================
      Функция парсинга опций, паттернов и файлов из аргументов командной строки:
          Реализация через структуру с отдельными переменными для опций
===================================================================================*/
// typedef struct {
//   bool opt_e;  // pattern
//   bool opt_i;  // non registr
//   bool opt_v;  // inverse
//   bool opt_c;  // string count only
//   bool opt_l;  // files only
//   bool opt_n;  // line number
//   bool opt_h;  // without filename
//   bool opt_s;  // without error messages about nonexistan or unreadable files
//   bool opt_f;  // regular expression from file
//   bool opt_o;  // output the matched parts only
//   bool pat_exist;  // pattern input flag
//   char** patterns;
//   char** files;
// } input;

// void opt_pat_def(char** argv, int index, input* data) {
//   int i = 1;  // Итератор по символам текущей строки argv[index]
//   while (argv[index][i] && !data->patterns[index]) {
//   // or while ( *(*(argv + index) + i) && !*(data->patterns + index)) {
//     char* psymb = *(argv + index) + i;

//     // opt_e analysis
//     if (*psymb == 'e') {
//       data->opt_e = 1;
//       if (*(psymb + 1) != '\0') {
//         data->patterns[index] = psymb + 1;
//         data->pat_exist = TRUE;
//       } else {
//         data->patterns[index + 1] = argv[index + 1];
//         data->pat_exist = TRUE;
//       }
//     }  // if (*psymb == 'e')

//     if (*psymb == 'i') data->opt_i = 1;
//     if (*psymb == 'v') data->opt_v = 1;
//     if (*psymb == 'c') data->opt_c = 1;
//     if (*psymb == 'l') data->opt_l = 1;
//     if (*psymb == 'n') data->opt_n = 1;
//     if (*psymb == 'h') data->opt_h = 1;
//     if (*psymb == 's') data->opt_s = 1;
//     if (*psymb == 'f') data->opt_f = 1;
//     if (*psymb == 'o') data->opt_o = 1;
//     i++;
//   }
// }
