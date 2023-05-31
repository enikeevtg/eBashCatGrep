/*  e_grep.c
 *  (c) T. Enikeev
 *  zeftyrst@student.21-school.ru
 *  opt_mask indices: 0 1 2 3 4 5 6 7 8 9
 *     short options: n o h e i v c l s f
 *  Коды ошибок errcode:
 *  1 - system memory access error
 *  2 - too few arguments
 *  3 - illegal option
 *  4 - no such file or directory (include -f oprion template file)
 *  5 - option requires argument (-e and -f)
 *  7 - target file is required
 *  6 - file processing error (close(fp) == 0)
 */

#ifndef E_GREP
#define E_GREP

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/e_string.h"

#define OPTS_NUM 10  // single-character options quantity
#define _GNU_SOURCE  // for getline() compilation
#define TRUE 1
#define FALSE 0

typedef struct {
  char* shopts;      // short options string
  bool* opt_mask;    // short options mask array
  bool t_exist;      // Template input existing flag
  char** templs;     // pointers to argv contains templates
  int templs_num;    // quantity of templates
  char** files;      // pointers to argv contains files
  int files_num;     // quantity of target files
  char** t_files;    // pointers to argv contains files contain templs
  int errcode;       // error code
  int error_ch;      // error symbol
  char* error_file;  // error file name
} data_t;            // INPUT DATA TYPE FOR ANALYSIS

typedef struct {
  int file_index;    // file number in files array
  int line_index;    // line number
  bool esc_file;     // escape file signal
  bool enabl_print;  // permission to print line
  bool enabl_opt_o;  // permission to print line num and filename for -o opt
  int cflags;        // for regcomp
  int eflags;        // for regexec
} formats_t;         // FOR MATCH SEARCHING TYPE

//---FUNCTIONS_DECLARATIONS---------------------------------------------------------
void e_grep(int argc, char** argv, data_t* dp);

void opt_def(int argc, char** argv, data_t* dp);
void opt_e(int argc, char** argv, data_t* dp, int index, int i);
void opt_f(int argc, char** argv, data_t* dp, int index, int i);

void nonopt_def(int argc, char** argv, data_t* dp);
void array_squeezing(int argc, char** array);

void t_file_check(data_t* dp, int i);
void file_closing(FILE* fp, data_t* dp);
void t_file_read(data_t* dp);

void del_dupl(int argc, char** array, int i);
void file_proc(FILE* fp, data_t* dp, int f_index);
bool line_proc(data_t* dp, char* line, regmatch_t* pmatch, size_t nmatch,
               formats_t* fmsp);
void line_print(data_t* dp, char* line, regmatch_t* pm, bool* match,
                formats_t* fmsp);
// void print_match(int argc, data_t* dp);
void error_print(data_t* dp);
void mem_free(int argc, data_t* dp);

#ifdef DEBUG
void opt_print(data_t* dp);
void array_print(char** array, int argc);
#endif  // DEBUG
//----------------------------------------------------------------------------------

#endif  // E_GREP
