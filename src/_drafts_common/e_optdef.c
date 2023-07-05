#include "e_optdef.h"

/*==============================================================================
          Функция определения опций программы из аргументов командной строки
==============================================================================*/
void opt_def(int argc, char** argv, int start_argv, int* errcode,
             char* error_ch, char* shopts, char** lopts, int lopts_num,
             int* flags_mask, int* nonopt_index) {
  int stop_argv = 0;
  for (int i = start_argv; i < argc && *errcode == 0 && stop_argv == 0; i++) {
    int argvi_len = e_strlen(argv[i]);  // Длина строки argv[i]

    // Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
    if (argv[i][0] == '-' && argv[i][1] != '-' && argvi_len > 1)
      shopt_ident(argv[i], shopts, flags_mask, errcode, error_ch);

    // Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
    if (argv[i][0] == '-' && argv[i][1] == '-' && argvi_len > 2 &&
        lopts_num > 0)
      lopt_ident(argv[i], lopts, lopts_num, flags_mask, errcode, error_ch);

    // Запись значения индекса элемента argv[i], где указан путь к файлу
    if ((argv[i][0] != '-') || (argv[i][0] == '-' && argvi_len == 1)) {
      *nonopt_index = i;
      stop_argv = 1;
    }
  }
}

/*==============================================================================
            Функция идентификации коротких аргументов командной строки
==============================================================================*/
void shopt_ident(char* argvi, char* shopts, int* flags_mask, int* errcode,
                 char* error_ch) {
  argvi++;
  while (*argvi && !*errcode) {
    char* opt_pos = e_strchr(shopts, *argvi);
    if (opt_pos == NULL) {
      *errcode = 1;
      *error_ch = *argvi;
    } else {
      flags_mask[opt_pos - shopts] = 1;
    }
    argvi++;
  }
}

/*==============================================================================
          Функция идентификации длинных аргументов командной строки
==============================================================================*/
void lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
                int* errcode, char* error_ch) {
  for (int i_mask = 0; i_mask < lopts_num && *errcode == 0; i_mask++) {
    if (e_strcmp(argvi, lopts[i_mask])) {
      *errcode = 1;
      *error_ch = '-';
    } else
      flags_mask[i_mask] = 1;
  }
}

// void lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
//                 int* errcode, char* error_ch) {
//   *error_ch = '-';
//   errcode = 1;
//   for (int i_mask = 0; i_mask < lopts_num && errcode != 0; i_mask++) {
//     if (!e_strcmp(argvi, lopts[i_mask])) {
//       flags_mask[i_mask]++;
//       errcode = 0;
//     }
//   }
// }
