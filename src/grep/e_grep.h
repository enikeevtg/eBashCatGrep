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
} data_t;

//---FUNCTIONS_DECLARATIONS---------------------------------------------------------
void e_grep(int argc, char** argv, data_t* dp);
void opt_def(int argc, char** argv, int index, data_t* dp);
void opt_e(int argc, char** argv, data_t* dp, int index, int i);
void opt_f(int argc, char** argv, data_t* dp, int index, int i);
void t_file_check(data_t* dp, int i);
void file_closing(FILE* fp, data_t* dp);
void t_file_read(int argc, data_t* dp, int i);
int del_dupl(int argc, char** str_array, int i);
void match_processing(FILE* fp, data_t* dp, int f_index);
// void print_match(int argc, data_t* dp);
void error_print(data_t* dp);
void mem_free(int argc, data_t* dp);

#ifdef DEBUG
void opt_print(data_t* dp);
void array_print(char** array, int argc);
#endif  // DEBUG
//----------------------------------------------------------------------------------

#endif  // E_GREP
