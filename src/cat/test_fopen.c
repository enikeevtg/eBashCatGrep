//  Тестирование функции fopen())
#include <stdio.h>

int main() {
    FILE* fp = stdin;
    while(!feof(fp)) {
        char ch = fgetc(fp);
        printf("%c", ch);
    }
    return 0;
}