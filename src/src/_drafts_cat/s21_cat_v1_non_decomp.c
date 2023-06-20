//  Реализация парсера для s21_cat.
//  Почти весь код лежит в main()

#include <stdbool.h>
#include <stdio.h>

#define SHOPTS_NUM 7
#define LOPTS_NUM 3

size_t s21_strlen(const char* str);
int s21_strcmp(const char* str1, const char* str2);

int main(int argc, char** argv) {
  int i_argv_FILE = 1;
  int errcode = 0;
  //  НАЧАЛО ИНИЦИАЛИЗАЦИИ МАССИВОВ ОПЦИЙ
  //  Массив односимвольных флагов (short options)
  char shopts_array[SHOPTS_NUM + 1] = "bnseEtT";
  //  Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие
  //  флаги b, n и s
  char lopt_b[18] = "--number-nonblank";
  char lopt_n[9] = "--number";
  char lopt_s[16] = "--squeeze-blank";
  char* lopts_array[LOPTS_NUM] = {lopt_b, lopt_n, lopt_s};
  //                                       1  2  3  4  5  6  7
  //  Массив нулей, соответствующий маске {b, n, s, e, E, t, T}
  bool flags_array[SHOPTS_NUM] = {0};
  //  КОНЕЦ ИНИЦИАЛИЗАЦИИ МАССИВОВ ОПЦИЙ

  //  НАЧАЛО ПАРСИНГА ОПЦИЙ
  for (int i_argv = 1; i_argv < argc && errcode == 0; i_argv++) {
    size_t string_length =
        s21_strlen(argv[i_argv]);  // Длина i-ой строки в массиве строк argv
    //  Идентификация флагов опций
    if (argv[i_argv][0] == '-' && string_length > 1) {
      //  Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
      if (argv[i_argv][1] != '-') {
        for (size_t i_char = 1; i_char < string_length && errcode == 0;
             i_char++) {
          errcode = 1;
          for (int i_flarr = 0; i_flarr < SHOPTS_NUM && errcode != 0;
               i_flarr++) {
            if (argv[i_argv][i_char] == shopts_array[i_flarr]) {
              flags_array[i_flarr] = 1;
              errcode = 0;
            }
          }
        }
        //  Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
      } else {
        errcode = 1;
        for (int i_flarr = 0; i_flarr < LOPTS_NUM && errcode != 0; i_flarr++) {
          if (!s21_strcmp(argv[i_argv], lopts_array[i_flarr])) {
            flags_array[i_flarr] = 1;
            errcode = 0;
          }
        }
      }
    } else {
      i_argv_FILE = i_argv;
    }
  }
  //  КОНЕЦ ПАРСИНГА ОПЦИЙ

  if (errcode != 0) {
    printf("errcode");
  } else {
    for (int i = 0; i < 7; i++) {
      printf(" %d", flags_array[i]);
      printf("\n%d", i_argv_FILE);
    }
  }

  return 0;
}

/*==============================================================================
                    Функция парсинга аргументов командной строки
==============================================================================*/

/*==============================================================================
                        Функция определения длины строки
==============================================================================*/
size_t s21_strlen(const char* str) {
  size_t count = 0;
  while (str[count] != '\0') count++;
  return count;
}

/*==============================================================================
                          Функция сравнения двух строк
==============================================================================*/
int s21_strcmp(const char* str1, const char* str2) {
  int count = 0;
  while (str1[count] == str2[count] && str1[count] != '\0') count++;
  return str1[count] - str2[count];
}

/*==============================================================================
                            Склад чернового кода
==============================================================================*/
// typedef struct {
//     bool OPT_b;
//     bool OPT_e;
//     bool OPT_E;
//     bool OPT_n;
//     bool OPT_s;
//     bool OPT_t;
//     bool OPT_T;
// } s21_cat_flags;

// for (int i = 0; i < argc; i++) {
//     printf("%s\n", *(argv + i));
// }

// printf("%d\n", argc);
// int i = 1;
// while (i < argc) {
//     printf("%s\n", *(argv + i));
//     i++;
// }

// char flags[10] = {0};
// for (int i = 1; i < argc; i++) {
//     if (argv[i][0] == "-") {
//         while (s21_strcmp())
//     }
// }
//     printf()

//    char lopts_array[3][18] = {"--number-nonblank", "--number",
//    "--squeeze-blank"};
