/*  e_cat
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - illegal option
 *  2 - no such file or directory
 *  3 - system memory error
 *
 *  Комментарий к обработке случая флага -e, -t, или -v:
 *  [NULL...Backspace] || [Vertical Tab...Unit Separator] ->
 *  -> ['@',...,'H'] || ['K'...'_']
 *  [0...8] || [11...31] -> [64...72] || [75...95]
 *  9 и 10 исключаются, так как это \n и \t:
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"  // e_str_len() & e_str_cmp()
#include "optdef.h"                // opt_def()

#define SHOPTS_NUM 8  // Количество односимвольных опций
#define LOPTS_NUM 3  // Количество многосимвольных опций

#define TRUE 1
#define FALSE 0

//---ПРОТОТИПЫ_ФУНКЦИЙ--------------------------------------------------------------
void e_cat(int argc, char** argv, data_t* pdata);
void print_ch(FILE* fp, int* opt_mask);
void error_print(data_t* pdata);
//-----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  // ИНИЦИАЛИЗАЦИЯ СТРУКТУРЫ ДАННЫХ ТИПА DATA_T
  data_t data = {0};
  data.shopts = "bsneEtTv";
  data.lopts = (char**)calloc(LOPTS_NUM, sizeof(char*));
  if (data.lopts != NULL) {
    *(data.lopts + 0) = "--number-nonblank";
    *(data.lopts + 1) = "--number-nonblank";
    *(data.lopts + 2) = "--number";
  } else
    data.error = 3;
  data.lopts_num = LOPTS_NUM;
  int array[SHOPTS_NUM] = {0};
  data.opt_mask = array;
  data.error = 0;
  data.error_ch = '\0';
  data.nonopt_index = 0;

  // ЗАПУСК ОСНОВНОЙ ФУНКЦИИ ПРОГРАММЫ
  if (!data.error)  // <=> if (data.error == 0)
    e_cat(argc, argv, &data);

  // ОБРАБОТКА ОШИБОК
  if (data.error)  // <=> if (data.error != 0)
    error_print(&data);

  // ОСВОБОЖДЕНИЕ ПАМЯТИ
  if (data.error != 3) free(data.lopts);

  return data.error;
}

/*===================================================================================
                              Реализация команды cat
===================================================================================*/
void e_cat(int argc, char** argv, data_t* pdata) {
  if (argc > 1) opt_def(argc, argv, pdata);

  for (int i = pdata->nonopt_index; i < argc && pdata->error == 0; i++) {
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
    Функция распечатки символа в терминал в соотеветствии с введёнными флагами:
                              0 1 2 3 4 5 6 7
                            " b s n e E t T v "
===================================================================================*/
void print_ch(FILE* fp, int* opt_mask) {
  char prev_prev_symb = '\0';  // Предпредыдущий прочитанный из файла символ
  char prev_symb = '\n';  // Предыдущий прочитанный из файла символ
  char symb = '\0';  // Текущий прочитанный из файла символ
  int line_counter = 0;
  bool print_access = TRUE;

  while ((symb = getc(fp)) != EOF) {
    // -b || --number-nonblank:
    if (opt_mask[0] && symb != '\n' && prev_symb == '\n')
      printf("%6d\t",
             ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // -s || --squeeze_blank:
    if (opt_mask[1] && symb == '\n' && prev_symb == '\n' &&
        prev_prev_symb == '\n')
      print_access = FALSE;

    // -n || --number, но нет (-b || --number-nonblank):
    if (opt_mask[2] && !opt_mask[0] && prev_symb == '\n' && print_access)
      printf("%6d\t",
             ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // Непечатаемые символы в -e и -t (флаг -v)
    if (opt_mask[3] || opt_mask[5] || opt_mask[7]) {
      if ((symb >= 0 && symb <= 8) || (symb >= 11 && symb <= 31)) {
        printf("^%c", symb + 64);
        print_access = FALSE;
      }
      if (symb == 127) {  // DEL -> '?' (63)
        printf("^%c", 63);
        print_access = FALSE;
      }
    }

    // -e || -E:
    if ((opt_mask[3] || opt_mask[4]) && symb == '\n' && print_access)
      printf("$");

    // -t || -T:
    if ((opt_mask[5] || opt_mask[6]) && symb == '\t') {
      printf("^I");
      print_access = FALSE;
    }

    if (print_access) putchar(symb);

    prev_prev_symb = prev_symb;
    prev_symb = symb;
    print_access = TRUE;
  }
}

/*===================================================================================
                              Обработка ошибок
===================================================================================*/
void error_print(data_t* pdata) {
  if (pdata->error == 1) {  // ОБРАБОТКА ОШИБКИ 1 illegal option
    fprintf(stderr, "e_cat: illegal option -- %c\n", pdata->error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", pdata->shopts);
  } else if (pdata->error == 3)
    fprintf(stderr, "e_cat: system memory error");
}
