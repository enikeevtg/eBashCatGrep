#ifndef e_OPTDEF_H
#define e_OPTDEF_H

int opt_def(int argc, char** argv, int start_argv, int* error, char* error_ch,
            char* shopts, int shopts_num, char** lopts, int lopts_num,
            int* flags_mask);  // opts_parsing()
int shopt_ident(char* argvi, int argvi_len, char* shopts, int shops_num,
                int* flags_mask, char* error_ch);
int lopt_ident(char* argvi, char** lopts, int lopts_num, int* flags_mask,
               char* error_ch);

#endif  // e_OPTDEF_H
