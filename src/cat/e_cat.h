/*  e_cat.h
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  Коды ошибок errcode:
 *  1 - illegal option
 *  2 - no such file or directory
 *  3 - system memory errcode
 *
 *  Комментарий к обработке случая флага -e, -t, или -v:
 *  [NULL...Backspace] || [Vertical Tab...Unit Separator] ->
 *  -> ['@',...,'H'] || ['K'...'_']
 *  [0...8] || [11...31] -> [64...72] || [75...95]
 *  9 и 10 исключаются, так как это \n и \t:
 */

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
  bool* opt_mask;  // Массив индикации введённых флагов
  int nonopt_index;  // Номер первого элемента не флага в argv
  int errcode;    // Код ошибки
  char error_ch;  // Символ ошибочного флага
} data_t;

//---ОБЪЯВЛЕНИЯ_ФУНКЦИЙ-------------------------------------------------------------
void data_init(data_t* dp);
void e_cat(int argc, char** argv, data_t* pdata);
void opt_def(int argc, char** argv, data_t* pdata);
void shopt_ident(char* argvi, data_t* pdata);
void lopt_ident(char* argvi, data_t* pdata);
void print_ch(FILE* fp, data_t* dp);
bool symb_analysis(data_t* dp, char symb, char prev_symb, char prev_prev_symb,
                   int* line_counter);
void error_print(data_t* pdata);
//-----------------------------------------------------------------------------------

#endif  // OPTDEF_H
