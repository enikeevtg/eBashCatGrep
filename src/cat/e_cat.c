/*  e_cat
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок error:
 *  1 - illegal option
 *  2 - no such file or directory
 */

#include <stdbool.h>
#include <stdio.h>

#define NUM_SHOPTS 8
#define NUM_LOPTS 3
#define TRUE 1
#define FALSE 0

//---ПРОТОТИПЫ_ФУНКЦИЙ---------------------------------------------------------------------------------------------
void e_cat(int argc, char** argv);
int opts_parsing(int argc, char** argv, int* error, char* error_ch,
                 char* shopts, char** lopts, bool* flags_mask);
int shopt_ident(char* argvi, size_t argvi_len, char* shopts, bool* flags_mask,
                char* error_ch);
int lopt_ident(char* argvi, char** lopts, bool* flags_mask, char* error_ch);

void print2stdout(int argc, char** argv, int FILE_index, bool* flags_mask);
void print_ch(FILE* fp, bool* flags_mask);

size_t e_strlen(const char* str);
int e_strcmp(const char* str1, const char* str2);
//-----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) { e_cat(argc, argv); }

/*=========================================================================================================
                                        Реализация команды cat
=========================================================================================================*/
void e_cat(int argc, char** argv) {
  int error = 0;
  char error_ch = '\0';
  int FILE_index = 0;

  //  Инициализация массивов флагов
  //  Массив односимвольных флагов (short options)
  char shopts[NUM_SHOPTS + 1] = "bsneEtTv";
  //  Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие
  //  флаги b, s и n
  char lopt_b[18] = "--number-nonblank";
  char lopt_s[16] = "--squeeze-blank";
  char lopt_n[9] = "--number";
  char* lopts[NUM_LOPTS] = {lopt_b, lopt_s, lopt_n};
  bool flags_mask[NUM_SHOPTS] = {0};  //  Массив индексов

  if (argc > 1)
    FILE_index =
        opts_parsing(argc, argv, &error, &error_ch, shopts, lopts, flags_mask);

  if (error == 0)
    print2stdout(argc, argv, FILE_index,
                 flags_mask /*, &error, &error_filename*/);
  else if (error == 1) {  //  ОБРАБОТКА ОШИБКИ illegal option
    fprintf(stderr, "s_21cat: illegal option -- %c\n", error_ch);
    fprintf(stderr, "usage: e_cat [-%s] [file ...]", shopts);
  }
}

/*=========================================================================================================
                            Функция распечатки файла в терминал
=========================================================================================================*/
void print2stdout(int argc, char** argv, int FILE_index,
                  bool* flags_mask /*, int* error, char** error_filename*/) {
  for (int i = FILE_index; i < argc; i++) {
    FILE* fp = stdin;  //  т.е. по умолчанию при отсутствии имени файла будем
                       //  читать stdin
    if (FILE_index < argc && e_strcmp(argv[i], "-") &&
        FILE_index > 0)  //  Проверка наличия имени файла
      fp = fopen(argv[i], "r");

    if (fp == NULL) {  //  ОБРАБОТКА ОШИБКИ no such file or directory
      fprintf(stderr, "s_21cat: %s: No such file or directory\n", argv[i]);
      //*error = 2;
      //*error_filename = argv[i];
    } else {
      print_ch(fp, flags_mask);
    }
    fclose(fp);
  }
}

/*=========================================================================================================
                Функция распечатки символа в терминал в соотеветствии с
введёнными флагами: 0 1 2 3 4 5 6 7 " b s n e E t T v"
=========================================================================================================*/
void print_ch(FILE* fp, bool* flags_mask) {
  char prev_prev_symb = '\0';
  char prev_symb = '\n';
  char symb = '\0';
  int nonempty_line_counter = 1;
  int line_counter = 1;
  bool print_access = TRUE;

  // while (fread(&symb, sizeof(char), 1, fp) > 0) {
  while ((symb = getc(fp)) != EOF) {
    //  -b || --number-nonblank
    if (flags_mask[0] && symb != '\n' && prev_symb == '\n') {
      printf("%6d\t", nonempty_line_counter);
      nonempty_line_counter++;
    }

    //  -s || --squeeze_blank
    if (flags_mask[1] && symb == '\n' && prev_symb == '\n' &&
        prev_prev_symb == '\n')
      print_access = FALSE;

    //  -n || --number, но нет -b || --number-nonblank
    if (flags_mask[2] && !flags_mask[0] && prev_symb == '\n' && print_access)
      printf("%6d\t", line_counter);

    //  Непечатаемые символы в -e и -t (флаг -v)
    //  [NULL...Backspace] || [Vertical Tab...Unit Separator] -> ['@',
    //  'A',...,'H'] || ['K'...'_'] [0...8] || [11...31] -> [64...72] ||
    //  [75...95] 9 и 10 исключаются, так как это \n и \t
    if (flags_mask[3] || flags_mask[5] || flags_mask[7]) {
      if ((symb >= 0 && symb <= 8) || (symb >= 11 && symb <= 31)) {
        printf("^%c", symb + 64);
        print_access = FALSE;
      }
      if (symb == 127) {  //  DEL -> '?' (63)
        printf("^%c", 63);
        print_access = FALSE;
      }
    }

    //  -e || -E
    if ((flags_mask[3] || flags_mask[4]) && symb == '\n' && print_access)
      printf("$");

    //  -t || -T
    if ((flags_mask[5] || flags_mask[6]) && symb == '\t') {
      printf("^I");
      print_access = FALSE;
    }

    if (print_access) putchar(symb);

    prev_prev_symb = prev_symb;
    prev_symb = symb;
    if (symb == '\n' &&
        print_access)  //  В случае наличия флага -s print_access здесь для
      line_counter++;  //  учёта сжатия при определении номера строки
    print_access = TRUE;
  }
}

/*=========================================================================================================
                            Функция парсинга аргументов командной строки
=========================================================================================================*/
int opts_parsing(int argc, char** argv, int* error, char* error_ch,
                 char* shopts, char** lopts, bool* flags_mask) {
  int FILE_index = 0;
  for (int i_argv = 1; i_argv < argc && *error == 0 && FILE_index == 0;
       i_argv++) {
    size_t argvi_len =
        e_strlen(argv[i_argv]);  // Длина i-ой строки в массиве строк argv[][]

    //  Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
    if (argv[i_argv][0] == '-' && argv[i_argv][1] != '-' && argvi_len > 1)
      *error =
          shopt_ident(argv[i_argv], argvi_len, shopts, flags_mask, error_ch);

    //  Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
    if (argv[i_argv][0] == '-' && argv[i_argv][1] == '-' && argvi_len > 2)
      *error = lopt_ident(argv[i_argv], lopts, flags_mask, error_ch);

    //  Запись значения индекса элемента argv[][], где прописывается путь к
    //  файлу
    if (argv[i_argv][0] != '-' && FILE_index == 0) {
      FILE_index = i_argv;
    }
    if (argv[i_argv][0] == '-' &&
        argvi_len ==
            1)  //  Если пользователь ввёл "... - ..." -> обработка stdin
      FILE_index = i_argv;
  }
  return FILE_index;
}

/*=========================================================================================================
                        Функция идентификации коротких аргументов командной
строки
=========================================================================================================*/
int shopt_ident(char* argvi, size_t argvi_len, char* shopts, bool* flags_mask,
                char* error_ch) {
  int error = 0;
  for (size_t i_char = 1; i_char < argvi_len && error == 0; i_char++) {
    error = 1;
    *error_ch = argvi[i_char];
    for (int i_flarr = 0; i_flarr < NUM_SHOPTS && error != 0; i_flarr++) {
      if (argvi[i_char] == shopts[i_flarr]) {
        flags_mask[i_flarr] = 1;
        error = 0;
      }
    }
  }
  return error;
}

/*=========================================================================================================
                        Функция идентификации длинных аргументов командной
строки
=========================================================================================================*/
int lopt_ident(char* argvi, char** lopts, bool* flags_mask, char* error_ch) {
  *error_ch = '-';
  int error = 1;
  for (int i_flarr = 0; i_flarr < NUM_LOPTS && error != 0; i_flarr++) {
    if (!e_strcmp(argvi, lopts[i_flarr])) {
      flags_mask[i_flarr] = 1;
      error = 0;
    }
  }
  return error;
}

/*=========================================================================================================
                                    Функция определения длины строки
=========================================================================================================*/
size_t e_strlen(const char* str) {
  size_t count = 0;
  while (str[count] != '\0') count++;
  return count;
}

/*=========================================================================================================
                                        Функция сравнения двух строк
=========================================================================================================*/
int e_strcmp(const char* str1, const char* str2) {
  int count = 0;
  while (str1[count] == str2[count] && str1[count] != '\0') count++;
  return str1[count] - str2[count];
}
