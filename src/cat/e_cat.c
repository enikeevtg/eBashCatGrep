/*  e_cat.c
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

#include "e_cat.h"

/*===================================================================================
                                      e_cat
===================================================================================*/
void e_cat(int argc, char** argv, data_t* pdata) {
  if (argc > 1) opt_def(argc, argv, pdata);

  for (int i = pdata->nonopt_index; i < argc && pdata->errcode == 0; i++) {
    FILE* fp = stdin;  // т.е. по умолчанию при отсутствии имени файла будем
                       // считывать stdin

    // Проверка наличия имени файла:
    if (pdata->nonopt_index < argc && pdata->nonopt_index > 0 &&
        e_strcmp(argv[i], "-"))
      fp = fopen(argv[i], "r");

    // ОБРАБОТКА ОШИБКИ 2 no such file or directory:
    if (fp == NULL)
      fprintf(stderr, "e_cat: %s: No such file or directory\n", argv[i]);
    else {
      print_ch(fp, pdata->opt_mask);
      fclose(fp);  // Закрытие файла
    }
  }
}

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

/*===================================================================================
    Функция распечатки символа в терминал в соотеветствии с введёнными флагами:
                                  0 1 2 3 4 5 6 7
                                " b s n e E t T v "
===================================================================================*/
void print_ch(FILE* fp, int* opt_mask) {
  char prev_prev_symb = '\0';  // Предпредыдущий прочитанный из файла символ
  char prev_symb = '\n';  // Предыдущий прочитанный из файла символ
  char symb = '\0';  // Текущий прочитанный из файла символ
  int line_counter = 0;
  bool enabl_print = TRUE;

  while ((symb = getc(fp)) != EOF) {
    // -b || --number-nonblank:
    if (opt_mask[0] && symb != '\n' && prev_symb == '\n')
      printf("%6d\t",
             ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // -s || --squeeze_blank:
    if (opt_mask[1] && symb == '\n' && prev_symb == '\n' &&
        prev_prev_symb == '\n')
      enabl_print = FALSE;

    // -n || --number, но нет (-b || --number-nonblank):
    if (opt_mask[2] && !opt_mask[0] && prev_symb == '\n' && enabl_print)
      printf("%6d\t",
             ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // Непечатаемые символы в -e и -t (флаг -v)
    if (opt_mask[3] || opt_mask[5] || opt_mask[7]) {
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
    if ((opt_mask[3] || opt_mask[4]) && symb == '\n' && enabl_print)
      printf("$");

    // -t || -T:
    if ((opt_mask[5] || opt_mask[6]) && symb == '\t') {
      printf("^I");
      enabl_print = FALSE;
    }

    if (enabl_print) putchar(symb);

    prev_prev_symb = prev_symb;
    prev_symb = symb;
    enabl_print = TRUE;
  }
}

/*===================================================================================
                              Обработка ошибок
===================================================================================*/
void error_print(data_t* pdata) {
  if (pdata->errcode == 1) {  // ОБРАБОТКА ОШИБКИ 1 illegal option
    fprintf(stderr, "e_cat: illegal option -- %c\n", pdata->error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", pdata->shopts);
  } else if (pdata->errcode == 3)
    fprintf(stderr, "e_cat: system memory access errcode");
}
