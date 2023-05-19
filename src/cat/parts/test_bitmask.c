//  Тестирование записи маски массива чисел

#include <stdio.h>
#include <math.h>
#define N 8

void print_bin(int8_t num);

int main() {
    int arr[N] = {5, -2, 8, 22, -11, -4, 0, 11};
    int8_t mask = 0b00000000;
    for (int i = 0; i < N; i++) {
        if (arr[i] >= 0)
            mask = mask | (int) pow(2, N - i - 1);
    }
    print_bin(mask);
    printf("\n%d\n0%o\n0x%X", mask, mask, mask);
    return 0;
}

//  Функция вывода в stdout двоичного представления восьмибитного целого числа
void print_bin(int8_t num) {
    printf("0b");
    for (int i = 0; i < 8; i++)
    printf("%c", (num & (0b10000000 >> i)) ? '1' : '0');
}


/*=======================================================
                Вывод байта в двоичном виде.
========================================================*/
/*
//  Взято с https://ru.stackoverflow.com/questions/944051/%D0%92%D1%8B%D0%B2%D0%BE%D0%B4-%D0%B4%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%BE%D0%B3%D0%BE-%D1%87%D0%B8%D1%81%D0%BB%D0%B0-%D0%B2-%D0%A1%D0%98

typedef unsigned char byte;

int main() {
    byte b = 93;  // Или любое другое
    for (int i = 0; i < 8; i++) {
    printf("%c", (b & 0x80) ? '1' : '0');
    b <<= 1;
    }
    printf("\n");
    return 0;
}
*/