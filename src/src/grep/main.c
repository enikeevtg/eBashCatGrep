/*  
 *  src/grep/main.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  zeftyrst@student.21-school.ru
 */

#include "s21_grep.h"

int main(int argc, char** argv) {
  // DATA_T DATA INITIALIZATION:
  data_t data = {0};  // input data struct init
  data_init(argc, &data);
  if (!data.templs || !data.files || !data.t_files) data.errcode = 1;
  // S21_GREP() FUNCTION LAUNCH:
  if (argc > 2 && !data.errcode)
    s21_grep(argc, argv, &data);
  else
    data.errcode = 2;
  // ERROR PROCESSING:
  if (data.errcode > 0)  // <=> if (data.errcode != 0)
    error_print(&data);
  // FREEING MEMORY:
  mem_free(argc, &data);
  return data.errcode;
}
