/*  cat/main.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 */

#include "e_cat.h"

int main(int argc, char** argv) {
  // ИНИЦИАЛИЗАЦИЯ СТРУКТУРЫ ДАННЫХ ТИПА DATA_T
  data_t data = {0};
  data.shopts = "bsneEtTv";
  data.lopts = (char**)calloc(LOPTS_NUM, sizeof(char*));
  if (data.lopts != NULL) {
    *(data.lopts + 0) = "--number-nonblank";
    *(data.lopts + 1) = "--number-nonblank";
    *(data.lopts + 2) = "--number";
  } else {
    data.errcode = 3;
  }
  data.lopts_num = LOPTS_NUM;
  int array[SHOPTS_NUM] = {0};
  data.opt_mask = array;
  data.errcode = 0;
  data.error_ch = '\0';
  data.nonopt_index = 0;

  // ЗАПУСК ОСНОВНОЙ ФУНКЦИИ ПРОГРАММЫ
  if (!data.errcode)  // <=> if (data.errcode == 0)
    e_cat(argc, argv, &data);

  // ОБРАБОТКА ОШИБОК
  if (data.errcode)  // <=> if (data.errcode != 0)
    error_print(&data);

  // ОСВОБОЖДЕНИЕ ПАМЯТИ
  if (data.errcode != 3) free(data.lopts);

  return data.errcode;
}
