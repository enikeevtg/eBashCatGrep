CC = gcc
CF = -c -Wall -Werror -Wextra
COM = ../common/
SRC = $(COM)e_string.c $(COM)e_optdef.c
OBJ = $(SRC:.c=.o)  #renaming SRC .c -> .o
EXE = e_cat

#  $@ Имя цели обрабатываемого правила
#  $< Имя первой зависимости обрабатываемого правила
#  $^ Список всех зависимостей обрабатываемого правила

e_cat: e_cat.o
	$(CC) e_cat.o $(COM)lib.a -o $(EXE)
	rm $^

test: e_cat
	sh test_cat/test_func_cat.sh

lib.a: $(OBJ)
	ar rv $(COM)$@ $^

e_cat.o: e_cat.c
	$(CC) $(CF) $^

e_string.o: $(COM)e_string.c
	$(CC) $(CF) $^ -o $(^:.c=.o)

e_optdef.o: $(COM)e_optdef.c
	$(CC) $(CF) $^ -o $(^:.c=.o)

clean:
	rm -rf $(EXE) $(OBJ) $(COM)lib.a

rebuild: clean e_cat
