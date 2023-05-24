/*  e_grep
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - system memory access error
 *  2 - too few arguments
 *  3 - illegal option
 *  4 - no such file or directory
 *  5 - option requires argument
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"

#define OPTS_NUM 10  // Количество односимвольных опций

#define TRUE 1
#define FALSE 0

typedef struct {
  char* shopts;  // short options string
  bool* opt_mask;
  bool pat_exist;  // pattern input flag
  char** templates;
  char** files;
  char** t_files;  // files contain templates
  int error;       // error code
  int error_ch;    // error symbol
} data_t;

//---ПРОТОТИПЫ_ФУНКЦИЙ-------------------------------------------------------------
void e_grep(int argc, char** argv, data_t* pdata);
void opt_pat_def(int argc, char** argv, int index, data_t* pdata);
void error_print(data_t* pdata);

void opt_print(data_t* pdata);
void array_print(char** array, int argc);
//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  // DATA_T DATA INITIALIZATION
  data_t data = {0};  // data_t is type of struct variable
  data.shopts = "eivclnhsfo";
  bool array[OPTS_NUM] = {0};
  data.opt_mask = array;
  data.pat_exist = FALSE;
  data.templates = (char**)calloc(argc, sizeof(char*));
  data.files = (char**)calloc(argc, sizeof(char*));
  data.t_files = (char**)calloc(argc, sizeof(char*));
  data.error = 0;
  data.error_ch = '\0';

  if (!data.templates || !data.files || !data.t_files) data.error = 1;

  // e_GREP() FUNCTION LAUNCH
  if (argc > 2 && !data.error)
    e_grep(argc, argv, &data);
  else
    data.error = 2;

  // FREEING MEMORY
  if (data.templates) free(data.templates);
  if (data.files) free(data.files);
  if (data.t_files) free(data.t_files);

  // ERROR PROCESSING
  if (data.error)  // <=> if (data.error != 0)
    error_print(&data);

  return data.error;
}

/*===================================================================================
                                    e_grep
===================================================================================*/
void e_grep(int argc, char** argv, data_t* pdata) {
  for (int i = 1; i < argc; i++) {
    // OPTION, TEMPLATE after -e or T_FILE after -f:
    if (**(argv + i) == '-') {  // or if (argv[i][0] == '-') {
      opt_pat_def(argc, argv, i, pdata);
      // printf("option\n");
    }
    // PATTERN:
    if (**(argv + i) != '-' && !pdata->pat_exist && !pdata->error) {
      *(pdata->templates + i) = *(argv + i);
      pdata->pat_exist = TRUE;
      // printf("pattern\n");
    }
    // FILE:
    if ((**(argv + i)) != '-' && (*(pdata->templates + i) == 0) &&
        !pdata->error) {
      *(pdata->files + i) = *(argv + i);
      // printf("file\n");
    }
  }

  opt_print(pdata);
  printf("\nTEMPLATES: ");
  array_print(pdata->templates, argc);
  printf("\nFILES: ");
  array_print(pdata->files, argc);
  printf("\n\n");
}

/*===================================================================================
      Функция парсинга опций, паттернов и файлов из аргументов командной строки:
          Реализация через массив (строку), в который помещены все флаги
===================================================================================*/
void opt_pat_def(int argc, char** argv, int index, data_t* pdata) {
  int i = 1;

  while (argv[index][i] && !pdata->templates[index] && !pdata->t_files[index] &&
         !pdata->error) {
    char* opt_pos = e_strchr(pdata->shopts, argv[index][i]);

    if (!opt_pos) {
      pdata->error = 3;
      pdata->error_ch = argv[index][i];
    } else {
      pdata->opt_mask[opt_pos - pdata->shopts] = 1;

      // -e processing
      if (*opt_pos == 'e') {
        if (argv[index][i + 1] != '\0') {
          pdata->templates[index] = argv[index] + i + 1;
          pdata->pat_exist = TRUE;
        } else if (index + 1 < argc) {
          pdata->templates[index + 1] = argv[index + 1];
          pdata->pat_exist = TRUE;
        } else {
          pdata->error = 5;
          pdata->error_ch = argv[index][i];
        }
      }
      // -f processing
      if (*opt_pos == 'f') {
        if (argv[index][i + 1] != '\0')
          pdata->t_files[index] = argv[index] + i + 1;
        else if (index + 1 < argc)
          pdata->t_files[index + 1] = argv[index + 1];
        else {
          pdata->error = 5;
          pdata->error_ch = argv[index][i];
        }
      }
    }
    i++;
  }
}

/*===================================================================================
                              Error processing
===================================================================================*/
// void error_print(data_t* pdata) {
//   if (pdata->error == 1) {
//     fprintf(stderr, "e_grep: system memory error\n");
//   } else if (pdata->error == 2) {
//     fprintf(stderr, "e_grep: Too few arguments\n");
//   } else if (pdata->error == 3) {
//     fprintf(stderr, "e_grep: illegal option -- %c\n", pdata->error_ch);
//     fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n",
//             pdata->shopts);
//   } else if (pdata->error == 4) {
//     fprintf(stderr, "e_grep: no such file or directory\n");
//   } else if (pdata->error == 5) {
//     fprintf(stderr, "e_grep: option requires an argument -- %c\n",
//             pdata->error_ch);
//     fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n",
//             pdata->shopts);
//   }
// }

//----для теста с перенаправлением результата в файл
void error_print(data_t* pdata) {
  if (pdata->error == 1) {
    fprintf(stdout, "e_grep: system memory access error\n\n");
  } else if (pdata->error == 2) {
    fprintf(stdout, "e_grep: Too few arguments\n\n");
  } else if (pdata->error == 3) {
    fprintf(stdout, "e_grep: illegal option -- %c\n", pdata->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n\n",
            pdata->shopts);
  } else if (pdata->error == 4) {
    fprintf(stdout, "e_grep: no such file or directory\n\n");
  } else if (pdata->error == 5) {
    fprintf(stdout, "e_grep: option requires an argument -- %c\n",
            pdata->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n",
            pdata->shopts);
  }
}



void opt_print(data_t* pdata) {
  printf("OPTIONS: ");
  for (int i = 0; i < OPTS_NUM; i++)
    if (pdata->opt_mask[i] == 1)
      printf("\t%c", pdata->shopts[i]);
}

void array_print(char** array, int argc) {
  for (int i = 0; i < argc; i++)
    if (array[i] != NULL) printf("%s\t", array[i]);
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
//   char** templates;
//   char** files;
// } data_t;

// void opt_pat_def(char** argv, int index, data_t* data) {
//   int i = 1;  // Итератор по символам текущей строки argv[index]
//   while (argv[index][i] && !data->templates[index]) {
//   // or while ( *(*(argv + index) + i) && !*(data->templates + index)) {
//     char* psymb = *(argv + index) + i;

//     // opt_e analysis
//     if (*psymb == 'e') {
//       data->opt_e = 1;
//       if (*(psymb + 1) != '\0') {
//         data->templates[index] = psymb + 1;
//         data->pat_exist = TRUE;
//       } else {
//         data->templates[index + 1] = argv[index + 1];
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

// void opt_print(data_t data) {
//   printf(
//       "\topt_e: %d\n\topt_i: %d\n\topt_v: %d\n\topt_c: %d\n\topt_l: "
//       "%d\n\topt_n: %d\n\topt_h: %d\n\topt_s: %d\n\topt_f: %d\n\topt_o:
//       %d\n", data.opt_e, data.opt_i, data.opt_v, data.opt_c, data.opt_l,
//       data.opt_n, data.opt_h, data.opt_s, data.opt_f, data.opt_o);
// }
