#ifndef OPTDEF_H
#define OPTDEF_H

typedef struct {
  char* shopts;  // Массив односимвольных флагов (short options)
  char** lopts;  // Массив многосимвольных флагов (long options)
  int lopts_num;
  int* flags_mask;  // Массив индикации введённых флагов
  int error;        // Код ошибки
  char error_ch;  // Символ ошибочного флага
  int nonopt_index;  // Номер первого элемента не флага в argv
} data_t;

void opt_def(int argc, char** argv, data_t* pdata);
void shopt_ident(char* argvi, data_t* pdata);
void lopt_ident(char* argvi, data_t* pdata);

#endif  // OPTDEF_H
