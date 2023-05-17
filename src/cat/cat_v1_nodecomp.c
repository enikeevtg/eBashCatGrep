//  Реализация парсера для cat.
//  Почти весь код лежит в main()

#include <stdio.h>
#include <stdbool.h>

#define NUM_SHOPTS 7
#define NUM_LOPTS 3

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);


int main(int argc, char** argv) {
    int i_argv_FILE = 1;
    int error = 0;
    //  НАЧАЛО ИНИЦИАЛИЗАЦИИ МАССИВОВ ОПЦИЙ
    //  Массив односимвольных флагов (short options)
    char shopts_array[NUM_SHOPTS + 1] = "bnseEtT";
    //  Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие флаги b, n и s
    char lopt_b[18] = "--number-nonblank";
    char lopt_n[9] = "--number";
    char lopt_s[16] = "--squeeze-blank";
    char* lopts_array[NUM_LOPTS] = {lopt_b, lopt_n, lopt_s};
    //                                       1  2  3  4  5  6  7
    //  Массив нулей, соответствующий маске {b, n, s, e, E, t, T}
    bool flags_array[NUM_SHOPTS] = {0};
    //  КОНЕЦ ИНИЦИАЛИЗАЦИИ МАССИВОВ ОПЦИЙ

    //  НАЧАЛО ПАРСИНГА ОПЦИЙ
    for(int i_argv = 1; i_argv < argc && error == 0; i_argv++) {
        size_t string_length = strlen(argv[i_argv]);  // Длина i-ой строки в массиве строк argv
        //  Идентификация флагов опций
        if(argv[i_argv][0] == '-' && string_length > 1) {
            //  Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
            if(argv[i_argv][1] != '-') {
                for(size_t i_char = 1; i_char < string_length && error == 0; i_char++) {

                    error = 1;
                    for(int i_flarr = 0; i_flarr < NUM_SHOPTS && error != 0; i_flarr++) {
                        if(argv[i_argv][i_char] == shopts_array[i_flarr]) {
                            flags_array[i_flarr] = 1;
                            error = 0;
                        }
                    }

                }            
            //  Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
            } else {

                error = 1;
                for(int i_flarr = 0; i_flarr < NUM_LOPTS && error != 0; i_flarr++) {
                    if(!strcmp(argv[i_argv], lopts_array[i_flarr])) {
                        flags_array[i_flarr] = 1;
                        error = 0;
                    }

                }                
            }            
        } else {
            i_argv_FILE = i_argv;
        }
    }
    //  КОНЕЦ ПАРСИНГА ОПЦИЙ

    if (error != 0) {
        printf("error");
    } else {
        for (int i = 0; i < 7; i++) {
            printf(" %d", flags_array[i]);
            printf("\n%d", i_argv_FILE);
        }
    }

    return 0;           
}





/*=======================================================
        Функция парсинга аргументов командной строки
========================================================*/








/*=======================================================
            Функция определения длины строки
========================================================*/
size_t strlen(const char* str) {
    size_t count = 0;
    while (str[count] != '\0')
        count++;
    return count;
}

/*=======================================================
            Функция сравнения двух строк
========================================================*/
int strcmp(const char* str1, const char* str2) {
    int count = 0;
    while(str1[count] == str2[count] && str1[count] != '\0')
        count++;
    return str1[count] - str2[count];
}











/*=======================================================
            Склад чернового кода
========================================================*/
// typedef struct {
//     bool OPT_b;
//     bool OPT_e;
//     bool OPT_E;
//     bool OPT_n;
//     bool OPT_s;
//     bool OPT_t;
//     bool OPT_T;
// } cat_flags;

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
    //         while (strcmp())
    //     }
    // }
    //     printf()

    //    char lopts_array[3][18] = {"--number-nonblank", "--number", "--squeeze-blank"};
