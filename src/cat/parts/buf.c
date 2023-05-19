/*  e_cat
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - illegal option
 *  2 - no such file or directory
 */

#include <stdbool.h>
#include <stdio.h>

#include "../common/e_optdef.h"  // opt_def()
#include "../common/e_string.h"  // e_str_len() & e_str_cmp()

#define NUM_SHOPTS 8  // Число односимвольных опций
#define NUM_LOPTS 3   // Число многосимвольных опций
#define TRUE 1
#define FALSE 0

//---ПРОТОТИПЫ_ФУНКЦИЙ-------------------------------------------------------------
void e_cat(int argc, char** argv);
void print2stdout(int argc, char** argv, int nonopt_index, bool* flags_mask);
void print_ch(FILE* fp, bool* flags_mask);
//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  e_cat(argc, argv);
  return 0;
}

/*===================================================================================
                              Реализация команды cat
===================================================================================*/
void e_cat(int argc, char** argv) {
  int error = 0;
  char error_ch = '\0';
  int nonopt_index = 0;
  int start_argv = 1;

  // ИНИЦИАЛИЗАЦИЯ МАССИВОВ ФЛАГОВ
  // Массив односимвольных флагов (short options):
  char shopts[NUM_SHOPTS + 1] = "bsneEtTv";
  // Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие:
  //                -b                   -s                 -n
  char* lopts[] = {"--number-nonblank", "--squeeze-blank", "--number"};
  // Массив индикации введённых флагов:
  bool flags_mask[NUM_SHOPTS] = {0};

  if (argc > 1)
    nonopt_index = opt_def(argc, argv, start_argv, &error, &error_ch, shopts,
                           NUM_SHOPTS, lopts, NUM_LOPTS, flags_mask);

  if (error == 0)
    print2stdout(argc, argv, nonopt_index, flags_mask);
  else if (error == 1) {  // ОБРАБОТКА ОШИБКИ illegal option
    fprintf(stderr, "s_21cat: illegal option -- %c\n", error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", shopts);
  }
}

/*===================================================================================
                      Функция распечатки файла в терминал
===================================================================================*/
void print2stdout(int argc, char** argv, int nonopt_index, bool* flags_mask) {
  for (int i = nonopt_index; i < argc; i++) {
    FILE* fp = stdin;  // т.е. по умолчанию при отсутствии имени файла будем
                       // считывать stdin

    // Проверка наличия имени файла:
    if (nonopt_index < argc && e_strcmp(argv[i], "-") && nonopt_index > 0)
          fp = fopen(argv[i], "r");

    // ОБРАБОТКА ОШИБКИ error = 2 no such file or directory:
    if (fp == NULL)
      fprintf(stderr, "s_21cat: %s: No such file or directory\n", argv[i]);
    else
      print_ch(fp, flags_mask);

    // Закрытие файла:
    if (fp != NULL)
      fclose(fp);

  }
}

/*===================================================================================
    Функция распечатки символа в терминал в соотеветствии с введёнными флагами:
                              0 1 2 3 4 5 6 7
                            " b s n e E t T v "
===================================================================================*/
void print_ch(FILE* fp, bool* flags_mask) {
  char prev_prev_symb = '\0';  // Предпредыдущий прочитанный из файла символ
  char prev_symb = '\n';  // Предыдущий прочитанный из файла символ
  char symb = '\0';  // Текущий прочитанный из файла символ
  int nonempty_line_counter = 1;
  int line_counter = 1;
  bool print_access = TRUE;

  // while (fread(&symb, sizeof(char), 1, fp) > 0) {
  while ((symb = getc(fp)) != EOF) {
    // -b || --number-nonblank:
    if (flags_mask[0] && symb != '\n' && prev_symb == '\n') {
      printf("%6d\t", nonempty_line_counter);
      nonempty_line_counter++;
    }

    // -s || --squeeze_blank:
    if (flags_mask[1] && symb == '\n' && prev_symb == '\n' &&
        prev_prev_symb == '\n')
      print_access = FALSE;

    // -n || --number, но нет -b || --number-nonblank:
    if (flags_mask[2] && !flags_mask[0] && prev_symb == '\n' && print_access)
      printf("%6d\t", line_counter);

    // Непечатаемые символы в -e и -t (флаг -v)
    // [NULL...Backspace] || [Vertical Tab...Unit Separator] -> ['@',
    // 'A',...,'H'] || ['K'...'_'] [0...8] || [11...31] -> [64...72] ||
    // [75...95] 9 и 10 исключаются, так как это \n и \t:
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
    if (symb == '\n' &&
        print_access)  // В случае наличия флага -s print_access здесь для
      line_counter++;  // учёта сжатия при определении номера строки
    print_access = TRUE;
  }
}
