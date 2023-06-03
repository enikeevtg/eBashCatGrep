/*==============================================================================
                              Склад чернового кода
==============================================================================*/

/*==============================================================================
    Функция парсинга опций, паттернов и файлов из аргументов командной строки:
        Реализация через структуру с отдельными переменными для опций
==============================================================================*/
// typedef struct {
//   bool opt_e;  // pattern
//   bool opt_i;  // non registr
//   bool opt_v;  // inverse
//   bool opt_c;  // string count only
//   bool opt_l;  // files only
//   bool opt_n;  // line number
//   bool opt_h;  // without filename
//   bool opt_s;  // without errcode messages about nonexistan or unreadable
//   files bool opt_f;  // regular expression from file bool opt_o;  // output
//   the matched parts only bool t_exist;  // pattern input flag char** templs;
//   char** files;
// } data_t;

// void opt_def(char** argv, int index, data_t* data) {
//   int i = 1;  // Итератор по символам текущей строки argv[index]
//   while (argv[index][i] && !data->templs[index]) {
//   // or while ( *(*(argv + index) + i) && !*(data->templs + index)) {
//     char* psymb = *(argv + index) + i;

//     // opt_e analysis
//     if (*psymb == 'e') {
//       data->opt_e = 1;
//       if (*(psymb + 1) != '\0') {
//         data->templs[index] = psymb + 1;
//         data->t_exist = TRUE;
//       } else {
//         data->templs[index + 1] = argv[index + 1];
//         data->t_exist = TRUE;
//       }
//     }  // if (*psymb == 'e')

//     if (*psymb == 'i') data->opt_i = 1;
//     if (*psymb == 'v') data->opt_v = 1;
//     if (*psymb == 'c') data->opt_c = 1;
//     if (*psymb == 'l') data->opt_l = 1;
//     if (*psymb == 'n') data->opt_n = 1;
//     if (*psymb == 'h') data->opt_h = 1;
//     if (*psymb == 's') data->opt_s = 1;
//     if (*psymb == 'f') data->opt_f = 1;
//     if (*psymb == 'o') data->opt_o = 1;
//     i++;
//   }
// }

// void opt_print(data_t data) {
//   printf(
//       "\topt_e: %d\n\topt_i: %d\n\topt_v: %d\n\topt_c: %d\n\topt_l: "
//       "%d\n\topt_n: %d\n\topt_h: %d\n\topt_s: %d\n\topt_f: %d\n\topt_o:
//       %d\n", data.opt_e, data.opt_i, data.opt_v, data.opt_c, data.opt_l,
//       data.opt_n, data.opt_h, data.opt_s, data.opt_f, data.opt_o);
// }

/*==============================================================================
                                SQUEEZING FUNCTION
==============================================================================*/
//   if (!dp->errcode) {
// #ifdef DEBUG
//   printf("squeezing start\n");
// #endif  // DEBUG
//     int j = 0;
//     for (int i = 0; j < dp->templs_num - 1; i++) {
//       while (!dp->templs[j] && j < dp->templs_num - 1) j++;
//       if (dp->templs[j]) {
//         dp->templs[i] = dp->templs[j];
//         dp->templs[j] = NULL;
//       }
//     }
// #ifdef DEBUG
//   printf("squeezing finish\n");
// #endif  // DEBUG
//   }

/*==============================================================================
                    Duplicates deletion function with squeezing
==============================================================================*/
// int del_dupl(int argc, char** str_array, int i) {
//   bool duplicate = FALSE;
//   // squeezing:
//   int j = 0;
//   while (str_array[j]) j++;
//   str_array[j] = str_array[i];
//   str_array[i] = NULL;
//   i = j;
//   // duplicates deletion:
//   for (int k = 0; k < i && !duplicate; k++) {
//     if (s21_strcmp(str_array[k], str_array[i]) == 0) {
//       duplicate = TRUE;
//       if (i >= argc)  // -> memory allocated by developer
//         free(str_array[i]);
//       str_array[i] = NULL;
//     }
//   }
//   return i;
// }  // 19 lines

/*==============================================================================
                Duplicates deletion function without squeezing
==============================================================================*/
// int del_dupl(int argc, char** str_array, int i) {
//   bool duplicate = FALSE;
//   for (int j = 0; j < i && !duplicate; j++) {
//     if (s21_strcmp(str_array[j], str_array[i]) == 0) {
//       duplicate = TRUE;
//       if (i >= argc)  // -> memory allocated by developer
//         free(str_array[i]);
//       str_array[i] = NULL;
//     }
//   }
//   return i;
// }  // 16 lines
