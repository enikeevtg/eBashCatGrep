//  Тестирование инициализации массива строк с многосимвольными флагами
#include <stdio.h>

int main() {
    char opt_b[18] = "--number-nonblank";
    char opt_n[9] = "--number";
    char opt_s[16] = "--squeeze-blank";
    char* array[3] = {opt_b, opt_n, opt_s};
    char word_array[3][18] = {"--number-nonblank", "--number", "--squeeze-blank"};
    for (int i = 0; i < 3; i++) {
        printf("%s\n", word_array[i]);
        printf("%s\n", array[i]);
    }

    return 0;
}
