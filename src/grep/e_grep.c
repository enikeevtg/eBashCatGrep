/*  e_grep
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - illegsl option
 *  2 - too few arguments
 *  3 - no such file or directory
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/e_string.h"

#define SHOPTS_NUM 6    // Количество односимвольных опций
#define LOPTS_NUM 0     // Количество многосимвольных опций
#define PATTERNS_NUM 3  // Количество паттернов на старте
#define FILES_NUM 3     // Количество файлов на старте

#define TRUE 1
#define FALSE 0


//---ПРОТОТИПЫ_ФУНКЦИЙ-------------------------------------------------------------
void e_grep(int argc, char** argv);
int nonopt_def(int argc, char** argv, int nonopt_index, int* patterns_mask, int* files_mask);
//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  if (argc > 2)
    e_grep(argc, argv);
  else
    fprintf(stderr, "e_grep: Too few arguments\n"); // ОБРАБОТКА ОШИБКИ 2
  return 0;
}


/*===================================================================================
                              Реализация команды grep
===================================================================================*/
void e_grep(int argc, char** argv) {
  int error = 0;
  char error_ch = '\0';
  int nonopt_index = 0;
  int start_argv = 1;

  // Инициализация массивов флагов
  char shopts[SHOPTS_NUM] = "eivcln";
  char* lopts[] = {};
  int flags_mask[SHOPTS_NUM] = {0};

  // Выделение памяти под символьные массивы паттернов и имён файлов:
  int* options_mask = (int*)calloc(argc, sizeof(int));
  int* patterns_mask = (int*)calloc(argc, sizeof(int));
  int* files_mask = (int*)calloc(argc, sizeof(int));

// Для grep нужно написать свой отдельный парсер!
  nonopt_index = opt_def(argc, argv, start_argv, &error, &error_ch, shopts,
                         SHOPTS_NUM, lopts, LOPTS_NUM, flags_mask);

  if (!error) {  // <=> if (error == 0)
    start_argv = nonopt_def(argc, argv, nonopt_index, patterns_mask, files_mask);
  } else if (error == 1) { {  // ОБРАБОТКА ОШИБКИ 1 illegal option
    fprintf(stderr, "e_grep: illegal option -- %c\n", error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", shopts);
  }

  }
  
  FILE* fp = fopen(argv[nonopt_index], "r");


}


/*===================================================================================
                      Функция определения паттернов и файлов
===================================================================================*/
int nonopt_def(int argc, char** argv, int nonopt_index, int* patterns_mask, int* files_mask) {

//  do {
    FILE* fp = fopen(argv[nonopt_index], "r");
    if (fp == NULL) {
      write_pattern();
    } else {
      write_filename();
    }

//  } while (*(argv++)[0]);
}
