//  cat
//  (c) T. Enikeev
//  zeftyrst@student.21-school.ru

//  Коды ошибок error:
//  1 - illegal option
//  2 - no such file or directory

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_SHOPTS 7
#define NUM_LOPTS 3

void cat_no_args();

void cat_full(int argc, char** argv);
int opts_parsing(int argc, char** argv, int* error, char* shopts_array, char** lopts_array, bool* index_array);
int shopt_ident(char* argvi, size_t argvi_len, char* shopts_array, bool* index_array);
int lopt_ident(char* argvi, char** lopts_array, bool* index_array);

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);



int main(int argc, char** argv) {
    if (argc == 1)
        cat_no_args();
    else
        cat_full(argc, argv);
    return 0;           
}



/*=========================================================================================================
                                Реализация команды cat без аргументов
=========================================================================================================*/
void cat_no_args() {
    ;
}


/*=========================================================================================================
                            Реализация команды cat при наличии аргументов
=========================================================================================================*/
void cat_full(int argc, char** argv) {
    int error = 0;
    //  Массив односимвольных флагов (short options)
    char shopts_array[NUM_SHOPTS + 1] = "bnseEtT";
    //  Массив многосимвольных флагов (long options), ДУБЛИРУЮЩИХ(!!!) короткие флаги b, n и s
    char lopt_b[18] = "--number-nonblank";
    char lopt_n[9] = "--number";
    char lopt_s[16] = "--squeeze-blank";
    char* lopts_array[NUM_LOPTS] = {lopt_b, lopt_n, lopt_s};
    bool index_array[NUM_SHOPTS] = {0};  //  Массив индексов


    int i_argv_FILE = opts_parsing(argc, argv, &error, shopts_array, lopts_array, index_array);
    if (i_argv_FILE == argc + 1)
        error = 2;  //  No such file or directory

    if (error != 0) {
        printf("error=%d", error);
    } else {
        for (int i = 0; i < 7; i++) {
            printf(" %d", index_array[i]);
        }
        printf("\n%d", i_argv_FILE);
    }
}


/*=========================================================================================================
                            Функция парсинга аргументов командной строки
=========================================================================================================*/
int opts_parsing(int argc, char** argv, int* error, char* shopts_array, char** lopts_array, bool* index_array) {
    int i_argv;
    for (i_argv = 1; i_argv < argc && *error == 0; i_argv++) {
        size_t argvi_len = strlen(argv[i_argv]);  // Длина i-ой строки в массиве строк argv[][]

        //  Проверка односимвольных флагов (в argv[i] НЕТ второго знака "-")
        if (argv[i_argv][0] == '-' && argv[i_argv][1] != '-' && argvi_len > 1)
            *error = shopt_ident(argv[i_argv], argvi_len, shopts_array, index_array);

        //  Проверка многосимвольных флагов (в argv[i] ЕСТЬ второй знак "-")
        if (argv[i_argv][0] == '-' && argv[i_argv][1] == '-' && argvi_len > 2) 
            *error = lopt_ident(argv[i_argv], lopts_array, index_array);
    }
    return i_argv;
}


/*=========================================================================================================
                        Функция идентификация коротких аргументов командной строки
=========================================================================================================*/
int shopt_ident(char* argvi, size_t argvi_len, char* shopts_array, bool* index_array) {
    int error = 0;
    for (size_t i_char = 1; i_char < argvi_len && error == 0; i_char++) {
        error = 1;
        for (int i_flarr = 0; i_flarr < NUM_SHOPTS && error != 0; i_flarr++) {
            if (argvi[i_char] == shopts_array[i_flarr]) {
                index_array[i_flarr] = 1;
                error = 0;
            }
//        printf("argv=%c opt=%d error=%d\n", argvi[i_char], shopts_array[i_flarr], error);
        }
    }

    return error;
}


int lopt_ident(char* argvi, char** lopts_array, bool* index_array) {
    int error = 1;
    for (int i_flarr = 0; i_flarr < NUM_LOPTS && error != 0; i_flarr++) {
        if (!strcmp(argvi, lopts_array[i_flarr])) {
            index_array[i_flarr] = 1;
            error = 0;
        }
    }
    return error;
}            




/*=========================================================================================================
                                    Функция определения длины строки
=========================================================================================================*/
size_t strlen(const char* str) {
    size_t count = 0;
    while (str[count] != '\0')
        count++;
    return count;
}

/*=========================================================================================================
                                        Функция сравнения двух строк
=========================================================================================================*/
int strcmp(const char* str1, const char* str2) {
    int count = 0;
    while (str1[count] == str2[count] && str1[count] != '\0')
        count++;
    return str1[count] - str2[count];
}











/*=========================================================================================================
                                            Склад чернового кода
=========================================================================================================*/
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

    // cc -Wextra -Wall -Werror -pedantic -Ofast -o cat Objects/main.c.o -LDependencies/jz/ -ljz
