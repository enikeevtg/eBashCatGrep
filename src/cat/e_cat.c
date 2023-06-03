/*
 *  src/cat/e_cat.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
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

#include "e_cat.h"

/*==============================================================================
                            Input data initialization
==============================================================================*/
void data_init(data_t* dp) {
  dp->shopts = "bsneEtTv";
  dp->lopts = (char**)calloc(LOPTS_NUM, sizeof(char*));
  if (dp->lopts != E_NULL) {
    *(dp->lopts + 0) = "--number-nonblank";
    *(dp->lopts + 1) = "--number-nonblank";
    *(dp->lopts + 2) = "--number";
  } else {
    dp->errcode = 3;
  }
  dp->lopts_num = LOPTS_NUM;
  dp->opt_mask = (bool*)calloc(SHOPTS_NUM, sizeof(bool));
  dp->errcode = 0;
  dp->error_ch = '\0';
  dp->nonopt_index = 0;
}

/*==============================================================================
                                  e_cat
==============================================================================*/
void e_cat(int argc, char** argv, data_t* dp) {
  if (argc > 1) opt_def(argc, argv, dp);

  for (int i = dp->nonopt_index; i < argc && dp->errcode == 0; i++) {
    FILE* fp = stdin;  // т.е. по умолчанию при отсутствии имени файла будем
                       // считывать stdin

    // Проверка наличия имени файла:
    if (dp->nonopt_index < argc && dp->nonopt_index > 0 &&
        e_strcmp(argv[i], "-"))
      fp = fopen(argv[i], "r");

    // ОБРАБОТКА ОШИБКИ 2 no such file or directory:
    if (fp == E_NULL)
      fprintf(stderr, "e_cat: %s: No such file or directory\n", argv[i]);
    else {
      print_ch(fp, dp);
      fclose(fp);  // Закрытие файла
    }
  }
}

/*==============================================================================
      Функция определения опций программы из аргументов командной строки
==============================================================================*/
void opt_def(int argc, char** argv, data_t* dp) {
  int stop_argv = 0;
  for (int i = 1; i < argc && !(dp->errcode) && !stop_argv; i++) {
    e_size_t argvi_len = e_strlen(argv[i]);  // Длина строки argv[i]

    // Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
    if (argv[i][0] == '-' && argv[i][1] != '-' && argvi_len > 1)
      shopt_ident(argv[i], dp);

    // Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
    if (argv[i][0] == '-' && argv[i][1] == '-' && argvi_len > 2 &&
        dp->lopts_num > 0)
      lopt_ident(argv[i], dp);

    // Запись значения индекса элемента argv[i], где указан путь к файлу
    if ((argv[i][0] != '-') || (argv[i][0] == '-' && argvi_len == 1)) {
      dp->nonopt_index = i;
      stop_argv = 1;
    }
  }
}

/*==============================================================================
          Функция идентификации коротких аргументов командной строки
==============================================================================*/
void shopt_ident(char* argvi, data_t* dp) {
  int i = 0;
  while (argvi[++i] && !dp->errcode) {
    char* opt_pos = e_strchr(dp->shopts, argvi[i]);
    if (opt_pos == E_NULL) {
      dp->errcode = 1;
      dp->error_ch = argvi[i];
    } else
      dp->opt_mask[opt_pos - dp->shopts] = 1;
  }
}

/*==============================================================================
            Функция идентификации длинных аргументов командной строки
==============================================================================*/
void lopt_ident(char* argvi, data_t* dp) {
  int match = dp->lopts_num;  // Счётчик совпадений
  for (int i = 0; i < dp->lopts_num; i++) {
    if (e_strcmp(argvi, dp->lopts[i]) == 0)
      dp->opt_mask[i] = 1;
    else
      match--;
  }
  if (!match) {
    dp->errcode = 1;
    dp->error_ch = '-';
  }
}

/*==============================================================================
    Функция распечатки символа в терминал в соответствии с введёнными флагами:
==============================================================================*/
void print_ch(FILE* fp, data_t* dp) {
  char prev_prev_symb = '\0';  // Предпредыдущий прочитанный из файла символ
  char prev_symb = '\n';  // Предыдущий прочитанный из файла символ
  char symb = '\0';  // Текущий прочитанный из файла символ
  int line_counter = 0;
  bool enabl_print = TRUE;

  while ((symb = getc(fp)) != EOF) {
    enabl_print =
        symb_analysis(dp, symb, prev_symb, prev_prev_symb, &line_counter);
    if (enabl_print) putchar(symb);
    prev_prev_symb = prev_symb;
    prev_symb = symb;
  }
}

/*==============================================================================
                          Функция анализа символа. Опции:
                                0 1 2 3 4 5 6 7
                              " b s n e E t T v "
==============================================================================*/
bool symb_analysis(data_t* dp, char symb, char prev_symb, char prev_prev_symb,
                   int* line_counter) {
  bool enabl_print = TRUE;
  // -b || --number-nonblank:
  if (dp->opt_mask[0] && symb != '\n' && prev_symb == '\n')
    printf("%6d\t", ++(*line_counter));  // сначала ++, затем -> stdin
  // -s || --squeeze_blank:
  if (dp->opt_mask[1] && symb == '\n' && prev_symb == '\n' &&
      prev_prev_symb == '\n')
    enabl_print = FALSE;
  // -n || --number, но нет (-b || --number-nonblank):
  if (dp->opt_mask[2] && !dp->opt_mask[0] && prev_symb == '\n' && enabl_print)
    printf("%6d\t", ++(*line_counter));  // сначала ++, затем -> stdin
  // Непечатаемые символы в -e и -t (флаг -v):
  if (dp->opt_mask[3] || dp->opt_mask[5] || dp->opt_mask[7]) {
    if ((symb >= 0 && symb <= 8) || (symb >= 11 && symb <= 31)) {
      printf("^%c", symb + 64);
      enabl_print = FALSE;
    }
    if (symb == 127) {  // DEL -> '?' (63)
      printf("^%c", 63);
      enabl_print = FALSE;
    }
  }
  // -e || -E:
  if ((dp->opt_mask[3] || dp->opt_mask[4]) && symb == '\n' && enabl_print)
    printf("$");
  // -t || -T:
  if ((dp->opt_mask[5] || dp->opt_mask[6]) && symb == '\t') {
    printf("^I");
    enabl_print = FALSE;
  }
  return enabl_print;
}

/*==============================================================================
                              Обработка ошибок
==============================================================================*/
void error_print(data_t* dp) {
  if (dp->errcode == 1) {  // ОБРАБОТКА ОШИБКИ 1 illegal option
    fprintf(stderr, "e_cat: illegal option -- %c\n", dp->error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", dp->shopts);
  } else if (dp->errcode == 3)
    fprintf(stderr, "e_cat: system memory access errcode");
}
