/*
 *  src/grep/s21_grep.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  zeftyrst@student.21-school.ru
 *  opt_mask indices: 0 1 2 3 4 5 6 7 8 9
 *     short options: n o h e i v c l s f
 *  Error codes:
 *  1 - system memory access error
 *  2 - too few arguments
 *  3 - illegal option
 *  4 - no such file or directory (include -f oprion template file)
 *  5 - option requires argument (-e and -f)
 *  7 - target file is required
 *  6 - file processing error (close(fp) == 0)
 */

#include "s21_grep.h"

/*==============================================================================
                          Input data initialization
==============================================================================*/
void data_init(int argc, data_t* dp) {
  dp->shopts = "noheivclsf";
  dp->opt_mask = (bool*)calloc(OPTS_NUM, sizeof(bool));
  dp->t_exist = FALSE;
  dp->templs = (char**)calloc(argc, sizeof(char*));
  dp->files = (char**)calloc(argc, sizeof(char*));
  dp->t_files = (char**)calloc(argc, sizeof(char*));
  dp->templs_num = argc;
  dp->errcode = 0;
  dp->error_ch = '\0';
}

/*==============================================================================
                                s21_grep
==============================================================================*/
void s21_grep(int argc, char** argv, data_t* dp) {
  // IDENTIFICATION OPT_MASK AND TEMPS after -e opt or T_FILES after -f opt:
  opt_def(argc, argv, dp);
  // IDENTIFICATION TEMPLATES AND FILES:
  if (!dp->errcode) {
    nonopt_def(argc, argv, dp);
    // FILES ARRAY SQUEEZING:
    array_squeezing(argc, dp->files);
  }
  // DUPLICATE T_FILES DELETION:
  if (!(*dp->files)) {
    dp->errcode = 6;  // target file is required
  } else {
    for (int i = 1; i < argc && !dp->errcode; i++) {
      while (!dp->t_files[i] && i < argc - 1) i++;
      if (dp->t_files[i]) del_dupl(argc, dp->t_files, i);
    }
    // T_FILES ARRAY SQUEEZING:
    array_squeezing(argc, dp->t_files);
    // READING T_FILES:
    t_file_read(dp);
  }
  // DUPLICATE TEMPLATES DELETION:
  for (int i = 1; i < dp->templs_num && !dp->errcode; i++) {
    while (!dp->templs[i] && i < dp->templs_num - 1) i++;
    if (dp->templs[i]) del_dupl(argc, dp->templs, i);
  }
  // FILES READING:
  FILE* fp = NULL;
  for (int f_index = 0; dp->files[f_index] && !dp->errcode; f_index++) {
    fp = fopen(dp->files[f_index], "r");
    if (fp)
      file_proc(fp, dp, f_index);
    else if (!dp->opt_mask[8])  // -s opt wasn't switched on
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              dp->files[f_index]);
    if (fp) file_closing(fp, dp);
  }
}

/*==============================================================================
                            Options parsing function
==============================================================================*/
void opt_def(int argc, char** argv, data_t* dp) {
  for (int i = 1; i < argc && !dp->errcode; i++)
    if (argv[i][0] == '-' && argv[i][1]) {
      // int j = 1;
      // while (argv[i][j] && !dp->templs[i] && !dp->t_files[i] && !dp->errcode)
      // {
      for (int j = 1;
           argv[i][j] && !dp->templs[i] && !dp->t_files[i] && !dp->errcode;
           j++) {
        // input symbol position in shopts:
        char* opt_pos = s21_strchr(dp->shopts, argv[i][j]);
        if (opt_pos == NULL) {
          dp->errcode = 3;
          dp->error_ch = argv[i][j];
        } else {
          dp->opt_mask[opt_pos - dp->shopts] = 1;
          if (*opt_pos == 'e') opt_e(argc, argv, dp, i, j);
          if (*opt_pos == 'f') opt_f(argc, argv, dp, i, j);
        }
        // j++;
      }
    }
  // -v => -o = 0:
  if (dp->opt_mask[5]) dp->opt_mask[1] = 0;
  // -c || -l => (-n && -o) = 0:
  if (dp->opt_mask[6] || dp->opt_mask[7])
    for (int i = 0; i < 2; i++) dp->opt_mask[i] = 0;
}

/*==============================================================================
                              Option -e processing
==============================================================================*/
void opt_e(int argc, char** argv, data_t* dp, int index, int i) {
  dp->t_exist = TRUE;
  if (argv[index][i + 1] != '\0') {
    dp->templs[index] = argv[index] + i + 1;
  } else if (index + 1 < argc) {
    dp->templs[index + 1] = argv[index + 1];
  } else {
    dp->errcode = 5;
    dp->error_ch = 'e';
  }
}

/*==============================================================================
                              Option -f processing
==============================================================================*/
void opt_f(int argc, char** argv, data_t* dp, int index, int i) {
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
}

/*==============================================================================
                        Tempate and files parsing function
==============================================================================*/
void nonopt_def(int argc, char** argv, data_t* dp) {
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] != '-' || (argv[i][0] == '-' && argv[i][1] == '\0')) &&
        !dp->t_exist) {
      dp->templs[i] = argv[i];
      dp->t_exist = TRUE;
    }
    if ((**(argv + i)) != '-' && !dp->templs[i] && !dp->t_files[i]) {
      dp->files[i] = argv[i];
      dp->files_num++;
    }
  }
}

/*==============================================================================
                          Array squeezing to array head
==============================================================================*/
void array_squeezing(int argc, char** array) {
  for (int i = 0, j = 0; i < argc && j < argc; i++, j++) {
    while (!array[i] && i < argc - 1) i++;
    while (array[j]) j++;
    if (j < i) {
      array[j] = array[i];
      array[i] = NULL;
    } else {
      j = i - 1;
    }
  }
}

/*==============================================================================
                          Duplicates deletion function
==============================================================================*/
void del_dupl(int argc, char** array, int i) {
  bool duplicate = FALSE;
  for (int j = 0; j < i && !duplicate; j++) {
    while (!array[j] && j < i - 1) j++;
    if (array[j] && s21_strcmp(array[j], array[i]) == 0) {
      duplicate = TRUE;
      if (i >= argc)  // -> memory allocated by developer
        free(array[i]);
      array[i] = NULL;
    }
  }
}

/*==============================================================================
                            Template file checking
==============================================================================*/
void t_file_check(data_t* dp, int i) {
  FILE* fp = fopen(dp->t_files[i], "r");
  if (!fp) {
    dp->errcode = 4;  // no such file or directory
    dp->error_file = dp->t_files[i];
  } else {
    file_closing(fp, dp);
  }
}

/*==============================================================================
                    Template file reading to templates array
==============================================================================*/
void t_file_read(data_t* dp) {
  ssize_t read_bytes = 0;
  char* line = NULL;
  size_t line_len = 64;

  for (int i = 0; dp->t_files[i]; i++) {
    // without NULL-checking due to t_file_check() done:
    FILE* fp = fopen(dp->t_files[i], "r");
    while ((read_bytes = getline(&line, &line_len, fp)) != -1 && !dp->errcode) {
      if (line[0] != '\n')
        line[read_bytes - 1] = '\0';  // character replacement: '\n' -> '\0'
      else
        read_bytes = 2;
      dp->templs_num++;
      dp->templs = (char**)realloc(dp->templs, dp->templs_num * sizeof(char*));
      if (dp->templs == NULL)
        dp->errcode = 1;  // system memory access error
      else
        dp->templs[dp->templs_num - 1] =
            (char*)calloc(read_bytes, sizeof(char));
      if (!dp->errcode && dp->templs[dp->templs_num - 1] == NULL)
        dp->errcode = 1;  // system memory access error
      else
        s21_strcpy(dp->templs[dp->templs_num - 1], line);  // insecure cause void!
    }
    free(line);
    // END OF READING LINES
    file_closing(fp, dp);
  }
}

/*==============================================================================
                                  File closing
==============================================================================*/
void file_closing(FILE* fp, data_t* dp) {
  if (fclose(fp) && !dp->opt_mask[8])  // if success, fclose returns 0
    fprintf(stderr, "s21_grep: %s: File processing error\n", dp->error_file);
}

/*==============================================================================
                    For matches search data initialization
==============================================================================*/
void formats_init(formats_t* fmsp, data_t* dp, int f_index) {
  fmsp->file_index = f_index;  // file number in files array
  fmsp->line_index = 0;        // line number
  fmsp->esc_file = FALSE;      // escape file signal
  fmsp->enabl_print = TRUE;
  // if -o, -c or -l opts was switched on -> line not printing:
  if (dp->opt_mask[6] || dp->opt_mask[7]) fmsp->enabl_print = FALSE;
  fmsp->enabl_opt_o = TRUE;
  fmsp->cflags = 0;  // for regcomp function
  // if -e opt was switched on:
  if (dp->opt_mask[3]) fmsp->cflags |= REG_EXTENDED;
  // if -i opt was switched on:
  if (dp->opt_mask[4]) fmsp->cflags |= REG_ICASE;
  fmsp->eflags = 0;  // for regexec function
}

/*==============================================================================
                                Match searching
==============================================================================*/
void file_proc(FILE* fp, data_t* dp, int f_index) {
  // for reading file lines by getline():
  ssize_t read_bytes = 0;
  char* line = NULL;
  size_t line_len = 64;

  int m_count = 0;      // matches counter
  formats_t fms = {0};  // struct for matches search init
  formats_init(&fms, dp, f_index);

  // for regexec:
  size_t nmatch = 1;
  regmatch_t pmatch[1] = {0};

  // START OF READING LINES FROM THE FILE
  while ((read_bytes = getline(&line, &line_len, fp)) != -1 && !fms.esc_file) {
    fms.line_index++;
    // if -n opt was switched on:
    if (dp->opt_mask[0]) fms.enabl_opt_o = TRUE;
    if (line_proc(dp, line, pmatch, nmatch, &fms)) m_count++;
  }
  free(line);  // END OF READING LINES FROM THE FILE

  // if -c opt was switched on:
  if (dp->opt_mask[6]) {
    if (!dp->opt_mask[2] && dp->files_num > 1)
      printf("%s:", dp->files[fms.file_index]);
    printf("%d\n", m_count);
  }
  // if -l opt was switched on:
  if (dp->opt_mask[7] && fms.esc_file) printf("%s\n", dp->files[f_index]);
}

/*==============================================================================
                          Line matching analysis
==============================================================================*/
bool line_proc(data_t* dp, char* line, regmatch_t* pmatch, size_t nmatch,
               formats_t* fmsp) {
  // LINE ANALYSIS:
  bool comp = FALSE;  // regex compilation regcomp() success sign
  bool match_found = FALSE;
  regex_t cur_templ;  // working good
  for (int i = 0; i < dp->templs_num && !match_found; i++) {
    while (!dp->templs[i] && i < dp->templs_num - 1) i++;
    if (dp->templs[i]) {
      // regex_t cur_templ;  // working good too
      comp = !regcomp(&cur_templ, dp->templs[i], fmsp->cflags);  // 0 is success
      if (comp)
        match_found = !regexec(&cur_templ, line, nmatch, pmatch, fmsp->eflags);
      regfree(&cur_templ);  // no leaks detected
    }
  }
  // regfree(&cur_templ);  // leaks detected!
  line_print(dp, line, pmatch, &match_found, fmsp);

  // if -o opt was switched on
  if (match_found && dp->opt_mask[1] && line[pmatch->rm_eo] != '\0') {
    fmsp->enabl_opt_o = FALSE;
    line_proc(dp, line + pmatch->rm_eo, pmatch, nmatch, fmsp);  // recursion
    fmsp->enabl_opt_o = TRUE;
  }
  return match_found;
}

/*==============================================================================
                            Line matching analysis
                         written: V V V V V V V V V V
                opt_mask indices: 0 1 2 3 4 5 6 7 8 9
                  short options: n o h e i v c l s f
==============================================================================*/
void line_print(data_t* dp, char* line, regmatch_t* pmatch, bool* match,
                formats_t* fmsp) {
  // if -v opt was switched on:
  if (dp->opt_mask[5]) *match = !*match;
  if (*match) {
    // if -l opt was switched on:
    if (dp->opt_mask[7]) fmsp->esc_file = TRUE;
    // if -h opt wasn't switched on:
    if (!dp->opt_mask[2] && dp->files_num > 1 && fmsp->enabl_opt_o &&
        fmsp->enabl_print)
      printf("%s:", dp->files[fmsp->file_index]);
    // if -n opt was switched on:
    if (dp->opt_mask[0] && fmsp->enabl_opt_o) printf("%d:", fmsp->line_index);
    // line printing if -o opt wasn't switched on:
    if (!dp->opt_mask[1] && fmsp->enabl_print) printf("%s", line);
    // line printing if -o opt was switched on:
    if (dp->opt_mask[1]) {
      for (regoff_t i = pmatch->rm_so; i < pmatch->rm_eo; i++)
        printf("%c", line[i]);
      printf("\n");
    }
  }
}

/*==============================================================================
                                Error processing
==============================================================================*/
void error_print(data_t* dp) {
  if (dp->errcode == 1) {
    fprintf(stderr, "s21_grep: System memory access error\n");
  } else if (dp->errcode == 2) {
    fprintf(stderr, "s21_grep: Too few arguments\n");
  } else if (dp->errcode == 3) {
    fprintf(stderr, "s21_grep: Illegal option -- %c\n", dp->error_ch);
    fprintf(stderr, "usage: s21_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 4) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n",
            dp->error_file);
  } else if (dp->errcode == 5) {
    fprintf(stderr, "s21_grep: Option requires an argument -- %c\n",
            dp->error_ch);
    fprintf(stderr, "usage: s21_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 6) {
    fprintf(stderr, "s21_grep: Target file is required\n");
    fprintf(stderr, "usage: s21_grep [-%s] [pattern] [file ...]\n", dp->shopts);
  } else if (dp->errcode == 7) {
    fprintf(stderr, "s21_grep: %s: File processing error\n", dp->error_file);
  }
}

/*==============================================================================
                                Freeing memory:
                    Maybe null-pointer checking not required
==============================================================================*/
void mem_free(int argc, data_t* dp) {
  free(dp->opt_mask);
  if (dp->templs) {
    for (int i = argc; i < dp->templs_num; i++)
      if (dp->templs[i]) free(dp->templs[i]);
    free(dp->templs);
  }
  if (dp->files) free(dp->files);
  if (dp->t_files) free(dp->t_files);
}
