/*  e_grep.c
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

#include "e_grep.h"

/*===================================================================================
                                    e_grep
===================================================================================*/
void e_grep(int argc, char** argv, data_t* dp) {
#ifdef DEBUG
  printf("e_grep was launched\n");
#endif  // DEBUG
  // OPTION, TEMPLATE after -e or T_FILE after -f:
  for (int i = 1; i < argc && !dp->errcode; i++)
    if (argv[i][0] == '-' && argv[i][1]) opt_def(argc, argv, i, dp);
  // if option -l detected
  if (dp->opt_mask[4]) {
    for (int i = 0; i < OPTS_NUM; i++) dp->opt_mask[i] = 0;
    dp->opt_mask[4] = 1;
  }

#ifdef DEBUG
  printf("patterns and files was launched\n");
#endif  // DEBUG
  // PATTERN AND TARGET FILES:
  for (int i = 1; i < argc && !dp->errcode; i++) {
    if ((argv[i][0] != '-' || (argv[i][0] == '-' && argv[i][1] == '\0')) &&
        !dp->t_exist) {
      dp->templs[i] = argv[i];
      dp->t_exist = TRUE;
    }
    if ((**(argv + i)) != '-' && !dp->templs[i] && !dp->t_files[i]) {
      dp->files[i] = argv[i];
    }
  }
#ifdef DEBUG
  printf("patterns and files has ended\n");
  printf("duplicate files deletion was launched\n");
#endif  // DEBUG
  // DUPLICATE TARGET FILES DELETION:
  for (int i = 1; i < dp->templs_num && !dp->errcode; i++) {
    while (!dp->files[i] && i < argc - 1) i++;
    if (dp->files[i]) del_dupl(argc, dp->files, i);
  }
#ifdef DEBUG
  printf("duplicate files deletion has ended\n");
  printf("duplicate t_files deletion was launched\n");
#endif  // DEBUG
  // READING T_FILES:
  for (int i = 1; i < argc && !dp->errcode; i++) {
    while (!dp->t_files[i] && i < argc - 1) i++;
    if (dp->t_files[i]) t_file_read(argc, dp, i);
  }
#ifdef DEBUG
  printf("t_files deletion has ended\n");
  printf("templates deletion was launched\n");
#endif  // DEBUG
  // DUPLICATE TEMPLATES DELETION:
  for (int i = 1; i < dp->templs_num && !dp->errcode; i++) {
    while (!dp->templs[i] && i < dp->templs_num - 1) i++;
    if (dp->templs[i]) del_dupl(argc, dp->templs, i);
  }
#ifdef DEBUG
  printf("temples deletion has ended\n");
#endif  // DEBUG

  FILE* fp = NULL;
  for (int i = 0; dp->files[i] && !dp->errcode; i++) {
    fp = fopen(dp->files[i], "r");
    if (fp)
      match_searching(fp, dp, i);
    else if (dp->files[i] && !fp)
      fprintf(stdout, "e_grep: %s: No such file or directory\n", dp->files[i]);
  }



#ifdef DEBUG
  printf("e_grep has ended\n");
#endif  // DEBUG
}

/*===================================================================================
                                Matches searching
===================================================================================*/
void match_searching(FILE* fp, data_t* dp, int index) {
#ifdef DEBUG
  printf("match_searching() was launched\n");
#endif  // DEBUG
  ssize_t read_bytes = 0;
  char* line = NULL;
  size_t line_len = 64;
  int line_num = 0;

  while ((read_bytes = getline(&line, &line_len, fp)) != -1) {
    line_num++;
    size_t nmatch = 1;
    regmatch_t pmatch[1] = {0};
    int cflags = REG_EXTENDED;
    int eflags = 0;
    for (int i = 0; dp->templs[i]; i++) {
      regex_t cur_templ;
      if (regcomp(&cur_templ, dp->templs[i], cflags) == 0) {  // success retuns 0
        if (regexec(&cur_templ, line, nmatch, pmatch, eflags) == 0) {
          // for (regoff_t j = pmatch->rm_so; j < pmatch->rm_eo; j++) 
          //   printf("\t\tregexec: j=%lld, %c\n", j, line[j]);
          printf("%s:%d:%s", dp->files[index], line_num, line);
        }
      regfree(&cur_templ);
      }      
    }
  }
  free(line);
#ifdef DEBUG
  printf("match_searching() has ended\n");
#endif  // DEBUG
}


/*===================================================================================
                            Options parsing function
===================================================================================*/
void opt_def(int argc, char** argv, int index, data_t* dp) {
#ifdef DEBUG
  printf("opt_def() was launched\n");
#endif  // DEBUG
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
  printf("opt_def() has ended\n");
#endif  // DEBUG
}

/*===================================================================================
                                Option -e processing
===================================================================================*/
void opt_e(int argc, char** argv, data_t* dp, int index, int i) {
#ifdef DEBUG
  printf("\t├──opt_e() was launched\n");
#endif  // DEBUG
  dp->t_exist = TRUE;
  if (argv[index][i + 1] != '\0') {
    dp->templs[index] = argv[index] + i + 1;
  } else if (index + 1 < argc) {
    dp->templs[index + 1] = argv[index + 1];
  } else {
    dp->errcode = 5;
    dp->error_ch = 'e';
  }
#ifdef DEBUG
  printf("\t└──opt_e() has ended\n");
#endif  // DEBUG
}

/*===================================================================================
                                Option -f processing
===================================================================================*/
void opt_f(int argc, char** argv, data_t* dp, int index, int i) {
#ifdef DEBUG
  printf("\t├──opt_f() was launched\n");
#endif  // DEBUG
  dp->t_exist = TRUE;
  if (index == argc - 1) {
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
  printf("\t└──opt_f() has ended\n");
#endif  // DEBUG
}

/*===================================================================================
                              Template file checking
===================================================================================*/
void t_file_check(data_t* dp, int i) {
  FILE* fp = fopen(dp->t_files[i], "r");
  if (!fp) {
    dp->errcode = 4;  // no such file or directory
    dp->error_file = dp->t_files[i];
  } else {
    file_closing(fp, dp, i);
  }
}

/*===================================================================================
                                    File closing
===================================================================================*/
void file_closing(FILE* fp, data_t* dp, int i) {
  if (fclose(fp)) {   // if success, fclose returns 0
    dp->errcode = 6;  // file processing error
    dp->error_file = dp->t_files[i];
    error_print(dp);
    dp->errcode = 0;
  }
}

/*===================================================================================
                      Template file reading to templates array
===================================================================================*/
void t_file_read(int argc, data_t* dp, int i) {
#ifdef DEBUG
  printf("\t├──t_file_read() was launched: %s\n", dp->t_files[i]);
#endif  // DEBUG
  i = del_dupl(argc, dp->t_files, i);
  FILE* fp = NULL;
  if (dp->t_files[i]) {
    fp = fopen(dp->t_files[i], "r");
    // START OF READING LINES FROM T_FILE TO TEMPLATES ARRAY:
    ssize_t read_bytes = 0;
    char* line = NULL;
    size_t line_len = 64;

    while ((read_bytes = getline(&line, &line_len, fp)) != -1 && !dp->errcode) {
      line[read_bytes - 1] = '\0';  // character replacement: '\n' -> '\0'
      dp->templs_num++;
      dp->templs = (char**)realloc(dp->templs, dp->templs_num * sizeof(char*));
      if (dp->templs == NULL)
        dp->errcode = 1;  // system memory access error
      else
        dp->templs[dp->templs_num - 1] = (char*)calloc(read_bytes, sizeof(char));
      if (!dp->errcode && dp->templs[dp->templs_num - 1] == NULL)
        dp->errcode = 1;  // system memory access error
      else
        e_strcpy(dp->templs[dp->templs_num - 1], line);  
    }
    free(line);
    // END OF READING LINES
    file_closing(fp, dp, i);
  }
#ifdef DEBUG
  printf("\t└──t_file_read() has ended: %s\n", dp->t_files[i]);
#endif  // DEBUG
}  // 27 lines

/*===================================================================================
                            Duplicates deletion function
===================================================================================*/
int del_dupl(int argc, char** str_array, int i) {
#ifdef DEBUG
  printf("\t├──del_dupl() was launched: %s\n", str_array[i]);
#endif  // DEBUG
  bool duplicate = FALSE;
  // squeezing:
  int j = 0;
  while (str_array[j]) j++;
  str_array[j] = str_array[i];
  str_array[i] = NULL;
  i = j;
  // duplicates deletion:
  for (int k = 0; k < i && !duplicate; k++) {
    if (e_strcmp(str_array[k], str_array[i]) == 0) {
      duplicate = TRUE;
      if (i >= argc)  // -> memory allocated by developer
        free(str_array[i]);
      str_array[i] = NULL;
#ifdef DEBUG
  printf("\t│   \t└──DUPLICATE DETECTED!\n");
#endif  // DEBUG
    }
  }
#ifdef DEBUG
  printf("\t└──del_dupl() has ended: %s\n", str_array[i]);
#endif  // DEBUG
  return i;
}  // 16 lines

/*===================================================================================
                              Error processing
===================================================================================*/
#ifndef DEBUG
void error_print(data_t* dp) {
  if (dp->errcode == 1) {
    fprintf(stderr, "e_grep: system memory access error\n");
  } else if (dp->errcode == 2) {
    fprintf(stderr, "e_grep: too few arguments\n");
  } else if (dp->errcode == 3) {
    fprintf(stderr, "e_grep: illegal option -- %c\n", dp->error_ch);
    fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 4) {
    fprintf(stderr, "e_grep: %s: no such file or directory\n",
            dp->error_file);
  } else if (dp->errcode == 5) {
    fprintf(stderr, "e_grep: option requires an argument -- %c\n",
            dp->error_ch);
    fprintf(stderr, "usage: e_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 6) {
    fprintf(stderr, "e_grep: %s: file processing error\n", dp->error_file);
  }
}
#else  // DEBUG
//---------------------------FOR_DEBUG_WITH_MAKEFILE_SCRIPT------------------------------
void error_print(data_t* dp) {
  if (dp->errcode == 1) {
    fprintf(stdout, "e_grep: system memory access error\n");
  } else if (dp->errcode == 2) {
    fprintf(stdout, "e_grep: too few arguments\n\n\n");
  } else if (dp->errcode == 3) {
    fprintf(stdout, "e_grep: illegal option -- %c\n", dp->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 4) {
    fprintf(stdout, "e_grep: %s: no such file or directory\n",
            dp->error_file);
  } else if (dp->errcode == 5) {
    fprintf(stdout, "e_grep: option requires an argument -- %c\n",
            dp->error_ch);
    fprintf(stdout, "usage: e_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 6) {
    fprintf(stdout, "e_grep: %s: file processing error\n", dp->error_file);
  }
}
#endif  // DEBUG

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

#ifdef DEBUG
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
#endif  // DEBUG
