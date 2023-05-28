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
  printf("e_grep start\n");
#endif
  // OPTION, TEMPLATE after -e or T_FILE after -f:
  for (int i = 1; i < argc && !dp->errcode; i++)
    if (argv[i][0] == '-' && argv[i][1]) opt_def(argc, argv, i, dp);
  // if option -l detected
  if (dp->opt_mask[4]) {
    for (int i = 0; i < OPTS_NUM; i++) dp->opt_mask[i] = 0;
    dp->opt_mask[4] = 1;
  }

// PATTERN AND TARGET FILES:
#ifdef DEBUG
  printf("patterns start\n");
#endif
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
  printf("patterns finish\n");
  printf("delete duplicate t_files start\n");
#endif
  // READING T_FILES:
  for (int i = 1; i < argc && !dp->errcode; i++) {
    while (!dp->t_files[i] && i < argc - 1) i++;
    if (dp->t_files[i]) t_file_read(argc, dp, i);
  }
#ifdef DEBUG
  printf("delete duplicate t_files finish\n");
  printf("delete duplicate templates start\n");
#endif
  // DUPLICATE TEMPLATES DELETION:
  for (int i = 1; i < dp->templs_num && !dp->errcode; i++) {
    while (!dp->templs[i] && i < dp->templs_num - 1) i++;
    if (dp->templs[i]) del_dupl(argc, dp->templs, i);
  }
#ifdef DEBUG
  printf("delete duplicates finish\n");
#endif

  FILE* fp = NULL;
  for (int i = 1; i < argc && !dp->errcode; i++) {
    while (!dp->files[i] && i < argc - 1) i++;
    fp = fopen(dp->files[i], "r");
    if (fp)
      match_searching(fp, dp);  
    else
      fprintf(stderr, "e_cat: %s: No such file or directory\n", dp->files[i]);
  }



#ifdef DEBUG
  printf("e_grep finish\n");
#endif
}

/*===================================================================================
                                Matches searching
===================================================================================*/
void print_match(FILE* fp, data_t* dp) {
  ssize_t read_bytes = 0;
  char* line = NULL;
  size_t line_len = 64;

  while (read_bytes = getline(&line, &line_len, fp) != -1) {
    regex_t pattern;
    for (int j = 1; j < dp->templs_num; j++) {
      while (!dp->files[j] && j < dp->templs_num - 1) j++;
      if (dp->files[j]) {

      }
    }
  }
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
  dp->t_exist = TRUE;
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
  printf("t_file_read start\n");
#endif
  del_dupl(argc, dp->t_files, i);
  FILE* fp = NULL;
  if (dp->t_files[i]) {
    fp = fopen(dp->t_files[i], "r");
    // READING LINES FROM T_FILE TO TEMPLATES ARRAY:
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
    free(line);  // END OF READING LINES
    file_closing(fp, dp, i);
  }
#ifdef DEBUG
  printf("t_file_read finish\n");
#endif
}  // 27 lines

/*===================================================================================
                            Duplicates deletion function
===================================================================================*/
void del_dupl(int argc, char** str_array, int i) {
#ifdef DEBUG
  printf("del_dupl start\n");
#endif
  bool duplicate = FALSE;
  for (int j = 1; j < i && !duplicate; j++) {
    while (!str_array[j] && j < i - 1) j++;
    if (str_array[j] && e_strcmp(str_array[j], str_array[i]) == 0) {
#ifdef DEBUG
      printf("%s\n", str_array[i]);
#endif
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
#else
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
#endif

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

void opt_print(data_t* dp) {
  printf("OPTIONS: ");
  for (int i = 0; i < OPTS_NUM; i++)
    if (dp->opt_mask[i] == 1) printf("\t%c", dp->shopts[i]);
}

void array_print(char** array, int argc) {
  for (int i = 0; i < argc; i++)
    if (array[i] != NULL) printf("%s\t", array[i]);
}
