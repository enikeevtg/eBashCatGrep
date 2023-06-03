#ifndef S21_OPTDEF_H
#define S21_OPTDEF_H

#include <stdio.h>
#include "../common/s21_string.h"

void opt_def(int argc, char** argv, int start_argv, int* errcode,
             char* error_ch, char* shopts, char** lopts, int lopts_num,
             int* flags_mask, int* nonopt_index);
void shopt_ident(char* argvi, char* shopts, int* errcode, int* flags_mask,
                 char* error_ch);
void lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
                int* errcode, char* error_ch);

#endif  // S21_OPTDEF_H
