/*==============================================================================
                              Склад чернового кода
==============================================================================*/
//  Структура для использования в getopts
// typedef struct {
//     bool OPT_b;
//     bool OPT_e;
//     bool OPT_E;
//     bool OPT_n;
//     bool OPT_s;
//     bool OPT_t;
//     bool OPT_T;
// } e_cat_flags;

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
//         while (e_strcmp())
//     }
// }
//     printf()

//    char lopts[3][18] = {"--number-nonblank", "--number", "--squeeze-blank"};

// Дебаг результата парсинга флагов
// for (int i = 0; i < 7; i++)
//     printf(" %d", flags_mask[i]);
// printf("\n%d", FILE_index);

// cc -Wextra -Wall -Werror -pedantic -Ofast -o e_cat Objects/main.c.o
// -LDependencies/jz/ -ljz

// void cat_no_args() {
//     char symb = '\0';
//     while ((symb = getchar()) != '\0')
//         putchar(symb);
// }

// void cat_no_args() {  //  ctrl + d: выводит в терминал ^D
//     int fd = STDIN_FILENO;  // fd = 0
//     char symb = '\0';
//     read(fd, &symb, 1);
//     while (symb > 0) {
//         putchar(symb);
//         read(fd, &symb, 1);
//     }
// }

// void cat_no_args() {  //  ctrl + d: бесконечный вывод в терминал
//     int fd = STDIN_FILENO;  // fd = 0
//     char symb = '\0';
//     fread(&symb, sizeof(char), 1, stdin);
//     while (symb > 0) {
//         putchar(symb);
//         fread(&symb, sizeof(char), 1, stdin);
//     }
// }

// void cat_no_args() { //  ctrl + d: норм
//     int fd = STDIN_FILENO;  // fd = 0
//     char symb = '\0';
//     while (scanf("%c", &symb) > 0)  //!= EOF
//         printf("%c", symb);
// }

// void cat_no_args() { //  ctrl + d: норм
//     int fd = STDIN_FILENO;  // fd = 0
//     char symb = '\0';
//     while ((symb = getchar()) > 0)  //!= EOF
//         putchar(symb);
// }

//  Начинал писать распечатку в терминал с использованием нестандартных
//  библиотек. В тележном чате этого проекта было обсуждение по поводу
//  необходимости использования таких библиотек, если в стандартнй все нужные
//  функции уже имеются. Начало функции print2stdout через стандартную
//  библиотеку (не стал использовать, так как низкоуровневый ввод-вывод
//  позволяет обращаться к файлам через дескриптор, и stdin в этом случае файл с
//  дескрипотором 0)
// FILE* fp;
// if ((fp = fopen(argv[FILE_index], r)) == NULL) {
//     *errcode = 2;
// } else {
//     while (!feof()) {
//         getc(fp);
//     }
// }

//#define BUFFER_SIZE 64  // Для ускорения чтения файла, если e_cat без флагов
// char buffer[BUFFER_SIZE];
// while (read(fd, buffer, BUFFER_SIZE) > 0)
//     fputs(buffer);

//
/*==============================================================================
        Распечатка, реализованная через низкоуровневые функции ввода/вывода
        библиотеки <unistd.h>
==============================================================================*/
// #include <unistd.h>  //  read(fd, buf, volume), STDIN_FILENO
// #include <fcntl.h>  //  open(&fname, flags), O_RDONLY

/*==============================================================================
                            Функция распечатки файла в терминал
==============================================================================*/
// void print2stdout(int argc, char** argv, int FILE_index, bool* flags_mask,
// int* errcode, char** error_filename) {
//     int fd = STDIN_FILENO;  //  Файловый дескриптор fd = 0, т.е. по умолчанию
//                             //  при отсутствии имени файла будем читать stdin
//     for (int i = FILE_index; i <= argc && *errcode == 0; i++) {
//         if (FILE_index != argc)  //  если пользователь ввёл хотя бы одно имя
//         файла
//             fd = open(argv[i], O_RDONLY);
//         if (fd < 0) {  //  если функция open() не смогла открыть файл и
//         вернуло значение -1
//             *errcode = 2;
//             *error_filename = argv[i];
//         } else {
//             print_ch(fd, flags_mask);
//         }
//         close(fd);
//     }
// }

/*==============================================================================
                Функция распечатки символа в терминал в соотеветствии с
                введёнными флагами:
                                          0 1 2 3 4 5 6
                                        " b s n e E t T "
==============================================================================*/
// void print_ch(int fd, bool* flags_mask) {
//     char prev_prev_symb = '\0';
//     char prev_symb = '\n';
//     char symb = '\0';
//     int nonempty_line_counter = 1;
//     int line_counter = 1;
//     bool enabl_print = TRUE;

//     while (read(fd, &symb, 1) > 0) {

//         //  -b || --number-nonblank
//         if (flags_mask[0] && symb != '\n' && prev_symb == '\n') {
//             char tmpstr[5] = "";
//             sprintf(tmpstr, "%d  ", nonempty_line_counter);
//             write(STDOUT_FILENO, tmpstr, 5);
//             nonempty_line_counter++;
//         }

//         //  -s || --squeeze_blank
//         if (flags_mask[1] && symb == '\n' && prev_symb == '\n' &&
//         prev_prev_symb == '\n')
//             enabl_print = FALSE;

//         //  -n || --number, но нет -b || --number-nonblank
//         if (flags_mask[2] && !flags_mask[0] && prev_symb == '\n' &&
//         enabl_print) {
//             char tmpstr[5] = "";
//             sprintf(tmpstr, "%d  ", line_counter);
//             write(STDOUT_FILENO, tmpstr, 5);
//         }

//         //  Непечатаемые символы в -e и -t (флаг -v)
//         //  [NULL...Backspace] || [Vertical Tab...Unit Separator] -> ['@',
//         'A',...,'H'] || ['K'...'_']
//         //  [0...8] || [11...31] -> [64...72] || [75...95]
//         //  9 и 10 исключаются, так как это \n и \t
//         if (flags_mask[3] || flags_mask[5]) {
//             if ((symb >= 0 && symb <= 8) || (symb >= 11 && symb <= 31)) {
//                 char tmpstr[2] = "";
//                 sprintf(tmpstr, "^%c", symb + 64);
//                 write(STDOUT_FILENO, tmpstr, 2);
//                 enabl_print = FALSE;
//             }
//             if (symb == 127) {  //  DEL -> '?' (63)
//                 char tmpstr[2] = "";
//                 sprintf(tmpstr, "^%c", 63);
//                 write(STDOUT_FILENO, tmpstr, 2);
//                 enabl_print = FALSE;
//             }
//         }

//         //  -e || -E
//         if ((flags_mask[3] || flags_mask[4]) && symb == '\n' && enabl_print)
//         {
//             write(STDOUT_FILENO, "$", 2);
//         }

//         //  -t || -T
//         if ((flags_mask[5] || flags_mask[6]) && symb == '\t') {
//             write(STDOUT_FILENO, "^I", 2);
//             enabl_print = FALSE;
//         }

//         if (enabl_print)
//             write(STDOUT_FILENO, &symb, 1);

//         prev_prev_symb = prev_symb;
//         prev_symb = symb;
//         if (symb == '\n' && enabl_print)  //  В случае наличия флага -s
//         enabl_print здесь...
//             line_counter++;                //  ...учитывает сжатие при
//             расчёте номера строки
//         enabl_print = TRUE;
//     }
// }

/*==============================================================================
                        Функция распечатки ошибок в терминал
==============================================================================*/
// void error_printing(int errcode, char error_ch, char* error_filename, char*
// shopts) {
//     if (errcode == 1) {
//         char error_message_illflag[36] = "";
//         char error_message_illflag_usage[32] = "usage: e_cat [-%s] [file
//         ...]"; sprintf(error_message_illflag, "s_21cat: illegal option --
//         %c\n", error_ch); sprintf(error_message_illflag_usage, "usage:
//         e_cat [-%s] [file ...]", shopts); write(STDERR_FILENO,
//         error_message_illflag, 36); write(STDERR_FILENO,
//         error_message_illflag_usage, 32);
//     }
//     if (errcode == 2) {
//         char error_message_nofile[101] = "";
//         sprintf(error_message_nofile, "\ns_21cat: %s:no such file or
//         directory", error_filename); write(STDERR_FILENO,
//         error_message_nofile, 36);
//     }
// }

/*==============================================================================
                      Функция распечатки ошибок в терминал
==============================================================================*/
// void error_printing(int errcode, char error_ch, char* error_filename, char*
// shopts) {
//     if (errcode == 1) {
//         printf("s_21cat: illegal option -- %c\n", error_ch);
//         printf("usage: e_cat [-%s] [file ...]", shopts);
//         // fprintf(stderr, "s_21cat: illegal option -- %c\n", error_ch);
//         // fprintf(stderr, "usage: e_cat [-%s] [file ...]", shopts);
//     }
//     if (errcode == 2)
//         printf("s_21cat: %s:no such file or directory", error_filename);
//         //fprintf(stderr, "s_21cat: %s:no such file or directory",
//         error_filename);  //  При выводе
//         // нескольких файлов одновременно, где второй файл несуществует, за
//         сообщением об этой ошибке
//         // выводится последняя строка предыдущего файла
// }

/*==============================================================================
                            Реализация команды cat
==============================================================================*/
// void e_cat(int argc, char** argv) {
// ...
// char lopt_b[18] = "--number-nonblank";
// char lopt_s[16] = "--squeeze-blank";
// char lopt_n[9] = "--number";
// char* lopts[NUM_LOPTS] = {lopt_b, lopt_s, lopt_n};
//                -b                   -s                 -n
// char* lopts[] = {"--number-nonblank", "--squeeze-blank", "--number"};
// Оказывается, так тоже работает! Видимо, всё дело в стандарте c11
// ...
// }

/*==============================================================================
          Функция идентификации коротких аргументов командной строки
==============================================================================*/
// void shopt_ident(char* argvi, data_t* pdata) {
//   argvi++;
//   while (*argvi && !pdata->errcode) {
//     char* opt_pos = e_strchr(pdata->shopts, *argvi);
//     if (opt_pos == NULL) {
//       pdata->errcode = 1;
//       pdata->error_ch = *argvi;
//     } else
//       pdata->flags_mask[opt_pos - pdata->shopts] = 1;
//     argvi++;
//   }
// }
