/*  
 *  src/grep/main.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 */

#include "e_grep.h"

int main(int argc, char** argv) {
#ifdef DEBUG
  printf("---------------------------------------------------------------\n");
#endif  // DEBUG
  // DATA_T DATA INITIALIZATION
  data_t data = {0};  // input data struct init
  data_init(argc, &data);

  if (!data.templs || !data.files || !data.t_files) data.errcode = 1;

  // E_GREP() FUNCTION LAUNCH
  if (argc > 2 && !data.errcode)
    e_grep(argc, argv, &data);
  else
    data.errcode = 2;

  // ERROR PROCESSING
  if (data.errcode > 0)  // <=> if (data.errcode != 0)
    error_print(&data);
#ifdef DEBUG
  else {
    printf("\n\tinput data: ");
    array_print(argv + 1, argc - 1);
    printf("\n\t");
    opt_print(&data);
    printf("\n\tTEMPLATES: ");
    array_print(data.templs, data.templs_num);
    printf("\n\tFILES: ");
    array_print(data.files, argc);
    printf("\n\tTEMPLATE FILES: ");
    array_print(data.t_files, argc);
  }
  printf("\n---------------------------------------------------------------\n");
#endif  // DEBUG

  // FREEING MEMORY
  mem_free(argc, &data);

#ifndef DEBUG
  return data.errcode;
#else
  return 0;
#endif
}
