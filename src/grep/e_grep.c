/*  e_grep
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  Коды ошибок errcode:
 *  1 - system memory access error
 *  2 - too few arguments
 *  3 - illegal option
 *  4 - no such file or directory (include -f oprion template file)
 *  5 - option requires argument (-e and -f)
 *  6 - file processing error (close(fp) == 0)
 */

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"

#define OPTS_NUM 10  // Количество односимвольных опций

#define TRUE 1
#define FALSE 0

//#define DEBUG

typedef struct {
  char* shopts;      // short options string
  bool* opt_mask;    // short options mask array
  bool t_xst;        // Template input eXiSTing flag
  char** templs;     // pointers to argv contains templates
  char** files;      // pointers to argv contains files
  char** t_files;    // pointers to argv contains files contain templs
  int templs_num;    // quantity of templates
  int errcode;       // error code
  int error_ch;      // error symbol
  char* error_file;  // error file name
} data_t;

//---ПРОТОТИПЫ_ФУНКЦИЙ-------------------------------------------------------------
void e_grep(int argc, char** argv, data_t* dp);
void opt_def(int argc, char** argv, int index, data_t* dp);
void opt_e(int argc, char** argv, data_t* dp, int index, int i);
void opt_f(int argc, char** argv, data_t* dp, int index, int i);
void t_file_check(data_t* dp, int i);
void t_file_read(int argc, data_t* dp, int i);
void del_dupl(int argc, char** str_array, int i);
// void match_search();
// void print_match(int argc, data_t* dp);
void error_print(data_t* dp);
void mem_free(int argc, data_t* dp);

void opt_print(data_t* dp);
void array_print(char** array, int argc);
//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
  // DATA_T DATA INITIALIZATION
  data_t data = {0};  // data_t is type of struct variable
  data.shopts = "eivclnhsfo";
  bool array[OPTS_NUM] = {0};
  data.opt_mask = array;
  data.t_xst = FALSE;
  data.templs = (char**)calloc(argc, sizeof(char*));
  data.files = (char**)calloc(argc, sizeof(char*));
  data.t_files = (char**)calloc(argc, sizeof(char*));
  data.templs_num = argc;
  data.errcode = 0;
  data.error_ch = '\0';

  if (!data.templs || !data.files || !data.t_files) data.errcode = 1;

  // E_GREP() FUNCTION LAUNCH
  if (argc > 2 && !data.errcode)
    e_grep(argc, argv, &data);
  else
    data.errcode = 2;

  // ERROR PROCESSING
  if (data.errcode > 0)  // <=> if (data.errcode != 0)
    error_print(&data);
  else {
    opt_print(&data);
    printf("\nTEMPLATES: ");
    array_print(data.templs, data.templs_num);
    printf("\nFILES: ");
    array_print(data.files, argc);
    printf("\nTEMPLATE FILES: ");
    array_print(data.t_files, argc);
  }

  // FREEING MEMORY
  mem_free(argc, &data);

printf("\n---------------------------------------------------------------\n");
  return 0;  // data.errcode;  NULL just for testing by Makefile script!
}

/*===================================================================================
                                    e_grep
===================================================================================*/
void e_grep(int argc, char** argv, data_t* dp) {
  #ifdef DEBUG
  printf("e_grep start\n");
  #endif
  // OPTION, TEMPLATE after -e or T_FILE after -f:
  for (int i = 1; i < argc && !dp->errcode; i++)
    if (argv[i][0] == '-' && argv[i][1])
      opt_def(argc, argv, i, dp);
  // if option -l detected
  if(dp->opt_mask[4]) {
    for (int i = 0; i < OPTS_NUM; i++)
      dp->opt_mask[i] = 0;
    dp->opt_mask[4] = 1;
  }

  // PATTERN AND TARGET FILES:
  #ifdef DEBUG
  printf("patterns start\n");
  #endif
  for (int i = 1; i < argc && !dp->errcode; i++) {
    if ((argv[i][0] != '-' || (argv[i][0] == '-' && argv[i][1] == '\0')) &&
        !dp->t_xst) {
      dp->templs[i] = argv[i];
      dp->t_xst = TRUE;
    }
    if ((**(argv + i)) != '-' && !dp->templs[i] && !dp->t_files[i]) {
      dp->files[i] = argv[i];
    }
  }
  #ifdef DEBUG
  printf("patterns finish\n");
  printf("delete duplicate t_files start\n");
  #endif
  // READING T_FILES:
  for (int i = 1; i < argc && !dp->errcode; i++) {
    while (!dp->t_files[i] && i < argc - 1) i++;
    if (dp->t_files[i]) t_file_read(argc, dp, i);
  }
  #ifdef DEBUG
  printf("delete duplicate templates start\n");
  #endif
  // DUPLICATE TEMPLATES DELETION:
  for (int i = 1; i < dp->templs_num && !dp->errcode; i++) {
    while (!dp->templs[i] && i < dp->templs_num - 1) i++;
    if (dp->templs[i]) del_dupl(argc, dp->templs, i);
  }
  #ifdef DEBUG
  printf("delete duplicates finish\n");
  printf("e_grep finish\n");
  #endif

  // FILE* fp = NULL;
  // if (!dp->errcode)
  //   for (int i = 1; i < argc; i++) {
  //     while (!dp->files[i])
  //       i++;
  //     fp = fopen(dp->files[i], "r");
  //     // ERROR 4 PROCESSING (no such file or directory):
  //     if (fp == NULL)
  //       fprintf(stderr, "e_cat: %s: No such file or directory\n", argv[i]);
  //     else {
  //       match_search();
  //       print_match(argc, dp);
  //       fclose(fp);  // Закрытие файла
  //   }
  // }
}

/*===================================================================================
                            Options parsing function
===================================================================================*/
void opt_def(int argc, char** argv, int index, data_t* dp) {
  #ifdef DEBUG
  printf("opt_def start\n");
  #endif
  int i = 1;
  while (argv[index][i] && !dp->templs[index] && !dp->t_files[index] &&
         !dp->errcode) {
    // input symbol position in shopts:
    char* opt_pos = e_strchr(dp->shopts, argv[index][i]);
    if (opt_pos == NULL) {
      dp->errcode = 3;
      dp->error_ch = argv[index][i];
    } else {
      dp->opt_mask[opt_pos - dp->shopts] = 1;
      if (*opt_pos == 'e') opt_e(argc, argv, dp, index, i);
      if (*opt_pos == 'f') opt_f(argc, argv, dp, index, i);
    }
    i++;
  }
  #ifdef DEBUG
  printf("opt_def finish\n");
  #endif
}

/*===================================================================================
                                Option -e processing
===================================================================================*/
void opt_e(int argc, char** argv, data_t* dp, int index, int i) {
  #ifdef DEBUG
  printf("opt_e start\n");
  #endif
  dp->t_xst = TRUE;
  if (argv[index][i + 1] != '\0') {
    dp->templs[index] = argv[index] + i + 1;
  } else if (index + 1 < argc) {
    dp->templs[index + 1] = argv[index + 1];
  } else {
    dp->errcode = 5;
    dp->error_ch = 'e';
  }
  #ifdef DEBUG
  printf("opt_e finish\n");
  #endif
}

/*===================================================================================
                                Option -f processing
===================================================================================*/
void opt_f(int argc, char** argv, data_t* dp, int index, int i) {
  #ifdef DEBUG
  printf("opt_f start\n");
  #endif
  dp->t_xst = TRUE;
  if (index + 1 == argc) {
    dp->errcode = 5;
    dp->error_ch = 'f';
  } else if (argv[index][i + 1] != '\0') {
    dp->t_files[index] = argv[index] + i + 1;
    t_file_check(dp, index);
  } else {
    dp->t_files[index + 1] = argv[index + 1];
    t_file_check(dp, index + 1);
  }
  #ifdef DEBUG
  printf("opt_f finish\n");
  #endif
}

/*===================================================================================
                              Template file checking
===================================================================================*/
void t_file_check(data_t* dp, int i) {
  FILE* fp = fopen(dp->t_files[i], "r");
  if (!fp) {
    dp->errcode = 4;  // no such file or directory
    dp->error_file = dp->t_files[i];
  }
}

/*===================================================================================
                      Template file reading to templates array
===================================================================================*/
void t_file_read(int argc, data_t* dp, int i) {
  #ifdef DEBUG
  printf("t_file_read start\n");
  #endif
  del_dupl(argc, dp->t_files, i);
  FILE* fp = NULL;
  if (dp->t_files[i] && (fp = fopen(dp->t_files[i], "r"))) {
    // READING LINES FROM T_FILE TO TEMPLATES ARRAY:
    ssize_t read_bytes;
    size_t line_len = 64;
    char* line = NULL;
    while ((read_bytes = getline(&line, &line_len, fp)) != -1 && !dp->errcode) {
      line[read_bytes - 1] = '\0';  // character replacement: '\n' -> '\0'
      dp->templs_num++;
      dp->templs = (char**)realloc(dp->templs, dp->templs_num * sizeof(char*));
      dp->templs[dp->templs_num - 1] = (char*)calloc(read_bytes, sizeof(char));
      if (dp->templs == NULL)
        dp->errcode = 1;  // system memory access error
      else
        e_strcpy(dp->templs[dp->templs_num - 1], line);
    }
    if (line) free(line);  // END OF READING LINES
    if (fclose(fp)) {  // if success, fclose returns 0
      dp->errcode = 6;  // file processing error
      dp->error_file = dp->t_files[i];
      error_print(dp);
      dp->errcode = 0;
    }
  }
  #ifdef DEBUG
  printf("t_file_read finish\n");
  #endif
}  // 27 lines

/*===================================================================================
                            Duplicate deletion function
===================================================================================*/
void del_dupl(int argc, char** str_array, int i) {
  #ifdef DEBUG
  printf("del_dupl start\n");
  printf("%s\n", str_array[i]);
  #endif
  bool duplicate = FALSE;
  for (int j = 1; j < i && !duplicate; j++) {
    while (!str_array[j] && j < i - 1) j++;
    if (str_array[j] && e_strcmp(str_array[j], str_array[i]) == 0) {
      if (i < argc) {
        str_array[i] = NULL;
        duplicate = TRUE;
      } else {
        free(str_array[i]);
        str_array[i] = NULL;
        duplicate = TRUE;
      }
    }
  }
  #ifdef DEBUG
  printf("del_dupl finish\n");
  #endif
}  // 16 lines

/*===================================================================================
                                Match output to stdout
===================================================================================*/
// void print_match(int argc, data_t* dp) {
//   regex_t reg_exp;
//   for (int i = 1; i < argc; i++) {
//     regcomp(&reg_exp, dp->templs[i], REG_EXTENDED);

//   }
// }

/*===================================================================================
                              Error processing
===================================================================================*/
// void error_print(data_t* dp) {
//   if (dp->errcode == 1) {
//     fprintf(stderr, "e_grep: system memory access error\n");
//   } else if (dp->errcode == 2) {
//     fprintf(stderr, "e_grep: too few arguments\n");
//   } else if (dp->errcode == 3) {
//     fprintf(stderr, "e_grep: illegal option -- %c\n", dp->error_ch);
//     fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n",
//     dp->shopts);
//   } else if (dp->errcode == 4) {
//     fprintf(stderr, "e_grep: %s: no such file or directory\n",
//             dp->error_file);
//   } else if (dp->errcode == 5) {
//     fprintf(stderr, "e_grep: option requires an argument -- %c\n",
//             dp->error_ch);
//     fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n",
//     dp->shopts);
//   } else if (dp->errcode == 6) {
//     fprintf(stderr, "e_grep: %s: file processing error\n", dp->error_file);
//   }
// }

/*===================================================================================
                                  Freeing memory:
                    Maybe null-pointer checking not required
===================================================================================*/
void mem_free(int argc, data_t* dp) {
  if (dp->templs) {
    for (int i = argc; i < dp->templs_num; i++) {
      if (dp->templs[i]) free(dp->templs[i]); 
    }
    free(dp->templs);
  }
  if (dp->files) free(dp->files);
  if (dp->t_files) free(dp->t_files);
}

//---------------------------FOR_TESTS_WITH_MAKEFILE_SCRIPT------------------------------
void error_print(data_t* dp) {
  if (dp->errcode == 1) {
    fprintf(stdout, "e_grep: system memory access error\n");
  } else if (dp->errcode == 2) {
    fprintf(stdout, "e_grep: too few arguments\n\n\n");
  } else if (dp->errcode == 3) {
    fprintf(stdout, "e_grep: illegal option -- %c\n", dp->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n",
            dp->shopts);
  } else if (dp->errcode == 4) {
    fprintf(stdout, "e_grep: %s: no such file or directory\n",
            dp->error_file);
  } else if (dp->errcode == 5) {
    fprintf(stdout, "e_grep: option requires an argument -- %c\n",
            dp->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n",
            dp->shopts);
  } else if (dp->errcode == 6) {
    fprintf(stdout, "e_grep: %s: file processing error\n", dp->error_file);
  } 
}

void opt_print(data_t* dp) {
  printf("OPTIONS: ");
  for (int i = 0; i < OPTS_NUM; i++)
    if (dp->opt_mask[i] == 1) printf("\t%c", dp->shopts[i]);
}

void array_print(char** array, int argc) {
  for (int i = 0; i < argc; i++)
    if (array[i] != NULL) 
      printf("%s\t", array[i]);
}
