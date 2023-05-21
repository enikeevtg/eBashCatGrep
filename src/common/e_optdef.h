#ifndef E_OPTDEF_H
#define E_OPTDEF_H

void opt_def(int argc, char** argv, int start_argv, int* error, char* error_ch,
            char* shopts, char** lopts, int lopts_num,
            int* flags_mask, int* nonopt_index);
void shopt_ident(char* argvi, char* shopts, int* error,
                 int* flags_mask, char* error_ch);
void lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
                int* error, char* error_ch);

#endif  // E_OPTDEF_H
