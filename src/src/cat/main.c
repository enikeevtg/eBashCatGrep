/*  
 *  src/cat/main.c
 *  (c) T. Enikeev
 *  enikeev.tg@gmail.com
 *  zeftyrst@student.21-school.ru
 */

#include "s21_cat.h"

int main(int argc, char** argv) {
  // ИНИЦИАЛИЗАЦИЯ СТРУКТУРЫ ДАННЫХ ТИПА DATA_T
  data_t data = {0};
  data_init(&data);
  // ЗАПУСК ОСНОВНОЙ ФУНКЦИИ ПРОГРАММЫ
  if (!data.errcode)  // <=> if (data.errcode == 0)
    s21_cat(argc, argv, &data);
  // ОБРАБОТКА ОШИБОК
  if (data.errcode)  // <=> if (data.errcode != 0)
    error_print(&data);
  // ОСВОБОЖДЕНИЕ ПАМЯТИ
  if (data.lopts) free(data.lopts);
  if (data.opt_mask) free(data.opt_mask);
  return data.errcode;
}
