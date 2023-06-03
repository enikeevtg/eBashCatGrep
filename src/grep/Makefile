# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                           #
#    src/grep/Makefile для проекта SimpleBashUtilits-1: e_grep            #
#    (c) T. Enikeev                                                         #
#    enikeev.tg@gmail.com                                                   #
#                                                                           #
#    Для справки:                                                           #
#    $@ Имя цели обрабатываемого правила                                    #
#    $< Имя первой зависимости обрабатываемого правила                      #
#    $^ Список всех зависимостей обрабатываемого правила                    #
#    $(SRC:.c=.o) - renaming SRC *.c -> *.o                                 #
#    $(^:.c=.o) - Список всех зависимостей обрабатываемого правила          #
#                 c переименованием расширений *.c -> *.o                   #
#                                                                           #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

CC = gcc
AR = ar rv
RAN = ranlib
RM = rm -rf

CF = -Wall -Werror -Wextra -c
STD = -std=c11 -pedantic

COM = ../common/
SRC = main.c e_grep.c
OBJ = main.o e_grep.o
LIB = e_string.a
EXE = e_grep


e_grep: $(OBJ) $(LIB)
	$(CC) $(OBJ) -o $(EXE) -L$(COM) $(COM)$(LIB)
	$(RM) *.o

main.o:
	$(CC) $(CF) main.c $(STD)

e_grep.o:
	$(CC) $(CF) e_grep.c $(STD) 

$(LIB): e_string.o
	$(AR) $(COM)$(LIB) $(COM)$^
	$(RAN) $(COM)$(LIB)
	$(RM) $^

e_string.o: 
	$(CC) $(CF) $(STD) $(COM)e_string.c -o $(COM)$@

style:
	clang-format -style=google -n *.h *.c $(COM)*.h $(COM)*.c

test:
	sh test_grep/test_func_grep.sh

leak:
	sh test_grep/test_leak_grep.sh

# linux VM needed!
valgrind:
	bash test_grep/test_ERROR_VALGRIND.sh

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) *.log

fclean: clean
	$(RM) $(COM)*.o
	$(RM) $(COM)$(LIB)

rebuild: clean e_grep

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                           #
#    Справка по созданию статической библиотеки:                            #
#    -L$(COM) - опция -L и адрес каталога относительно Makefile,            #
#              где при компоновке искать файл библиотеки                    #
#    -lmylib - опция -l и имя файла библиотеки libmylib.a без расширения и  #
#              префикса lib для подключения библиотеки к программе          #
#                                                                           #
#    ar - вызов архиватора для создания архива, который может подключаться  #
#         к программе во время компоновки на правах библиотеки              #
#    Опции архиватора:                                                      #
#    r - cоздаёт архив, добавляет или заменяет файлы в существующем архиве  #
#    x - извлекаются файлы из архива                                        #
#    c - приостанавливает вывод сообщений о том, что создаётся библиотека   #
#    v - включается режим подробных сообщений                               #
#                                                                           #
#    ranlib - пocтpoeниe oглaвлeния библиoтeки                              #
#                                                                           #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
