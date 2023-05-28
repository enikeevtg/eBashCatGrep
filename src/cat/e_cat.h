#ifndef OPTDEF_H
#define OPTDEF_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"  // e_str_len() & e_str_cmp()

#define SHOPTS_NUM 8  // Количество односимвольных опций
#define LOPTS_NUM 3  // Количество многосимвольных опций

#define TRUE 1
#define FALSE 0

typedef struct {
  char* shopts;  // Массив односимвольных флагов (short options)
  char** lopts;  // Массив многосимвольных флагов (long options)
  int lopts_num;
  int* opt_mask;  // Массив индикации введённых флагов
  int nonopt_index;  // Номер первого элемента не флага в argv
  int errcode;    // Код ошибки
  char error_ch;  // Символ ошибочного флага
} data_t;

//---ПРОТОТИПЫ_ФУНКЦИЙ--------------------------------------------------------------
void e_cat(int argc, char** argv, data_t* pdata);
void print_ch(FILE* fp, int* opt_mask);
void opt_def(int argc, char** argv, data_t* pdata);
void shopt_ident(char* argvi, data_t* pdata);
void lopt_ident(char* argvi, data_t* pdata);
void error_print(data_t* pdata);
//-----------------------------------------------------------------------------------

#endif  // OPTDEF_H
