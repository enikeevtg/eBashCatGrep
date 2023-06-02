# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#  																																					#
#    Makefile для проекта SimpleBashUtilits-1: e_grep                       #
#    (c) T. Enikeev                                                         #
#    enikeev.tg@gmail.com                                                   #
#    enikeev.tg@gmail.com                                                   #
#				 																																	  #
#    Для справки:																													  #
#    $@ Имя цели обрабатываемого правила																		#
#    $< Имя первой зависимости обрабатываемого правила											#
#    $^ Список всех зависимостей обрабатываемого правила										#
#    $(SRC:.c=.o) - renaming SRC *.c -> *.o																	#
#    $(^:.c=.o) - Список всех зависимостей обрабатываемого правила					#
#                 c переименованием расширений *.c -> *.o										#
#				 																																	  #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

CC = gcc
CF = -c -Wall -Werror -Wextra
COM = ../common/
SRC = main.c e_grep.c
OBJ = main.o e_grep.o
LIB = String
EXE = e_grep
STD = -std=c11 -pedantic
DEB = -D DEBUG

.PHONY: e_grep

e_grep: main.o e_grep.o lib
	$(CC) -o $(EXE) $(OBJ) -L$(COM) -l$(LIB)
	rm *.o

test:
	sh test_grep/test_func_grep.sh

leak:
	sh test_grep/test_leak_grep.sh

# linux VM needed!
valgrind:
	bash test_grep/test_ERROR_VALGRIND.sh

main.o:
	$(CC) $(CF) main.c $(STD)

e_grep.o:
	$(CC) $(CF) e_grep.c $(STD) 
# При использовании функции getline()
# необходимо либо в опциях компилятора
# прописывать -D_GNU_SOURCE,
# либо в коде перед вызвом <stdio.h>
# прописывать #define _GNU_SOURCE.
# *В зависимсти от версии:
# до glibc 2.10: _GNU_SOURCE
# начиная с glibc 2.10% _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700

lib: $(COM)e_string.o
	ar rv $(COM)libString.a $^
	rm $^

$(COM)e_string.o: 
	$(CC) $(CF) $(COM)e_string.c -o $@ $(STD)

clean:
	rm -rf $(EXE) $(OBJ) $(COM)libString.a

rebuild: clean e_grep

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				 																																	  #
#    Справка по созданию статической библиотеки: 														#
#    -L$(COM) - опция -L и адрес каталога относительно Makefile,						#
#              где при компоновке искать файл библиотеки										#
#    -lmylib - опция -l и имя файла библиотеки libmylib.a без расширения и	#
#              префикса lib для подключения библиотеки к программе	    		#
#				 																																	  #
#    ar - вызов архиватора для создания архива, который может подключаться  #
#         к программе во время компоновки на правах библиотеки              #
#    Опции архиватора:																											#
#    r - cоздаёт архив, добавляет или заменяет файлы в существующем архиве  #
#    x - извлекаются файлы из архива																				#
#    c - приостанавливает вывод сообщений о том, что создаётся библиотека   #
#    v - включается режим подробных сообщений																#
#				 																																	  #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
