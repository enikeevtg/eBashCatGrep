/*  e_cat
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - illegal option
 *  2 - no such file or directory
 */

#include <stdbool.h>
#include <stdio.h>

#include "optdef.h"  // opt_def()
#include "../common/e_string.h"  // e_str_len() & e_str_cmp()

#define SHOPTS_NUM 8  // Количество односимвольных опций
#define LOPTS_NUM 3   // Количество многосимвольных опций

#define TRUE 1
#define FALSE 0

//---ПРОТОТИПЫ_ФУНКЦИЙ--------------------------------------------------------------
void e_cat(int argc, char** argv);
void print2stdout(int argc, char** argv, int nonopt_index, int* flags_mask);
void print_ch(FILE* fp, int* flags_mask);
//-----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  e_cat(argc, argv);
  return 0;
}

/*===================================================================================
                              Реализация команды cat
===================================================================================*/
void e_cat(int argc, char** argv) {
  int error = 0;
  char error_ch = '\0';  // Символ ошибочного флага
  int nonopt_index = 0;
  int start_argv = 1;

  // ИНИЦИАЛИЗАЦИЯ МАССИВОВ ФЛАГОВ
  // Массив односимвольных флагов (short options):
  char shopts[SHOPTS_NUM + 1] = "bsneEtTv";
  // Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие:
  //                -b                   -s                 -n
  char* lopts[] = {"--number-nonblank", "--squeeze-blank", "--number"};
  // Массив индикации введённых флагов:
  int flags_mask[SHOPTS_NUM] = {0};

  if (argc > 1)
    opt_def(argc, argv, start_argv, &error, &error_ch, shopts, lopts, LOPTS_NUM, flags_mask, &nonopt_index);

  if (!error)  // <=> if (error == 0)
    print2stdout(argc, argv, nonopt_index, flags_mask);
  else if (error == 1) {  // ОБРАБОТКА ОШИБКИ 1 illegal option
    fprintf(stderr, "e_cat: illegal option -- %c\n", error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", shopts);
  }
}

/*===================================================================================
                      Функция распечатки файла в терминал
===================================================================================*/
void print2stdout(int argc, char** argv, int nonopt_index, int* flags_mask) {
  for (int i = nonopt_index; i < argc; i++) {
    FILE* fp = stdin;  // т.е. по умолчанию при отсутствии имени файла будем
                       // считывать stdin

    // Проверка наличия имени файла:
    if (nonopt_index < argc && e_strcmp(argv[i], "-") && nonopt_index > 0)
      fp = fopen(argv[i], "r");

    // ОБРАБОТКА ОШИБКИ 2 no such file or directory:
    if (fp == NULL)
      fprintf(stderr, "e_cat: %s: No such file or directory\n", argv[i]);
    else
      print_ch(fp, flags_mask);

    // Закрытие файла:
    if (fp != NULL) fclose(fp);
  }
}

/*===================================================================================
    Функция распечатки символа в терминал в соотеветствии с введёнными флагами:
                              0 1 2 3 4 5 6 7
                            " b s n e E t T v "
===================================================================================*/
void print_ch(FILE* fp, int* flags_mask) {
  char prev_prev_symb = '\0';  // Предпредыдущий прочитанный из файла символ
  char prev_symb = '\n';  // Предыдущий прочитанный из файла символ
  char symb = '\0';  // Текущий прочитанный из файла символ
  int line_counter = 0;
  bool print_access = TRUE;

  // while (fread(&symb, sizeof(char), 1, fp) > 0) {
  while ((symb = getc(fp)) != EOF) {
    // -b || --number-nonblank:
    if (flags_mask[0] && symb != '\n' && prev_symb == '\n')
      printf("%6d\t", ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // -s || --squeeze_blank:
    if (flags_mask[1] && symb == '\n' && prev_symb == '\n' &&
        prev_prev_symb == '\n')
      print_access = FALSE;

    // -n || --number, но нет -b || --number-nonblank:
    if (flags_mask[2] && !flags_mask[0] && prev_symb == '\n' && print_access)
      printf("%6d\t", ++line_counter);  // ++line_counter: сначала ++, затем -> stdin

    // Непечатаемые символы в -e и -t (флаг -v)
    // [NULL...Backspace] || [Vertical Tab...Unit Separator] -> ['@',...,'H']
    // || ['K'...'_'] [0...8] || [11...31] -> [64...72] || [75...95]
    // 9 и 10 исключаются, так как это \n и \t:
    if (flags_mask[3] || flags_mask[5] || flags_mask[7]) {
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
    if ((flags_mask[3] || flags_mask[4]) && symb == '\n' && print_access)
      printf("$");

    // -t || -T:
    if ((flags_mask[5] || flags_mask[6]) && symb == '\t') {
      printf("^I");
      print_access = FALSE;
    }

    if (print_access) putchar(symb);

    prev_prev_symb = prev_symb;
    prev_symb = symb;
    print_access = TRUE;
  }
}
