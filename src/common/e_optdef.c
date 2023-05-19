#include "e_optdef.h"
#include "e_string.h"

/*===================================================================================
          Функция определения опций программы из аргументов командной строки
===================================================================================*/
int opt_def(int argc, char** argv, int start_argv, int* error, char* error_ch,
            char* shopts, int shopts_num, char** lopts, int lopts_num,
            int* flags_mask) {
  int nonopt_index = 0;
  int stop_argv = 0;
  for (int i = start_argv; i < argc && *error == 0 && stop_argv == 0; i++) {
    int argvi_len = e_strlen(argv[i]);  // Длина строки argv[i]

    // Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
    if (argv[i][0] == '-' && argv[i][1] != '-' && argvi_len > 1)
      *error = shopt_ident(argv[i], argvi_len, shopts, shopts_num, flags_mask,
                           error_ch);

    // Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
    if (argv[i][0] == '-' && argv[i][1] == '-' && argvi_len > 2)
      *error = lopt_ident(argv[i], lopts, lopts_num, flags_mask, error_ch);

    // Запись значения индекса элемента argv[i], где указан путь к файлу
    if ((argv[i][0] != '-' && nonopt_index == 0) ||
        (argv[i][0] == '-' && argvi_len == 1)) {
      nonopt_index = i;
      stop_argv = 1;
    }
  }
  return nonopt_index;
}

/*===================================================================================
            Функция идентификации коротких аргументов командной строки
===================================================================================*/
int shopt_ident(char* argvi, int argvi_len, char* shopts, int shopts_num,
                int* flags_mask, char* error_ch) {
  int error = 0;
  for (int i_char = 1; i_char < argvi_len && error == 0; i_char++) {
    error = 1;
    *error_ch = argvi[i_char];
    for (int i_mask = 0; i_mask < shopts_num && error != 0; i_mask++) {
      if (argvi[i_char] == shopts[i_mask]) {
        flags_mask[i_mask] = 1;
        error = 0;
      }
    }
  }
  return error;
}

/*===================================================================================
          Функция идентификации длинных аргументов командной строки
===================================================================================*/
int lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
               char* error_ch) {
  *error_ch = '-';
  int error = 1;
  for (int i_mask = 0; i_mask < lopts_num && error != 0; i_mask++) {
    if (!e_strcmp(argvi, lopts[i_mask])) {
      flags_mask[i_mask] = 1;
      error = 0;
    }
  }
  return error;
}
