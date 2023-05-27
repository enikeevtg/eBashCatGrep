#include "optdef.h"

#include <stdio.h>

#include "../common/e_string.h"

/*===================================================================================
          Функция определения опций программы из аргументов командной строки
===================================================================================*/
void opt_def(int argc, char** argv, data_t* pdata) {
  int stop_argv = 0;
  for (int i = 1; i < argc && !(pdata->errcode) && !stop_argv; i++) {
    int argvi_len = e_strlen(argv[i]);  // Длина строки argv[i]

    // Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
    if (argv[i][0] == '-' && argv[i][1] != '-' && argvi_len > 1)
      shopt_ident(argv[i], pdata);

    // Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
    if (argv[i][0] == '-' && argv[i][1] == '-' && argvi_len > 2 &&
        pdata->lopts_num > 0)
      lopt_ident(argv[i], pdata);

    // Запись значения индекса элемента argv[i], где указан путь к файлу
    if ((argv[i][0] != '-') || (argv[i][0] == '-' && argvi_len == 1)) {
      pdata->nonopt_index = i;
      stop_argv = 1;
    }
  }
}

/*===================================================================================
            Функция идентификации коротких аргументов командной строки
===================================================================================*/
void shopt_ident(char* argvi, data_t* pdata) {
  int i = 0;
  while (argvi[++i] && !pdata->errcode) {
    char* opt_pos = e_strchr(pdata->shopts, argvi[i]);
    if (opt_pos == NULL) {
      pdata->errcode = 1;
      pdata->error_ch = argvi[i];
    } else
      pdata->opt_mask[opt_pos - pdata->shopts] = 1;
  }
}

/*===================================================================================
          Функция идентификации длинных аргументов командной строки
===================================================================================*/
void lopt_ident(char* argvi, data_t* pdata) {
  int match = pdata->lopts_num;  // Счётчик совпадений
  for (int i = 0; i < pdata->lopts_num; i++) {
    if (e_strcmp(argvi, pdata->lopts[i]) == 0)
      pdata->opt_mask[i] = 1;
    else
      match--;
  }
  if (!match) {
    pdata->errcode = 1;
    pdata->error_ch = '-';
  }
}
