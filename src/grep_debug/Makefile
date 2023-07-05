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
DEB = -D DEBUG

COM = ../common/
SRC = main.c e_grep.c
OBJ = main.o e_grep.o
LIB = e_string.a
EXE = e_grep


e_grep: $(OBJ) $(LIB)
	$(CC) $(OBJ) -o $(EXE) -L$(COM) $(COM)$(LIB)
	$(RM) *.o

main.o:
	$(CC) $(CF) $(DEB) main.c $(STD)

e_grep.o:
	$(CC) $(CF) $(DEB) e_grep.c $(STD) 

$(LIB): e_string.o
	$(AR) $(COM)$(LIB) $(COM)$^
	$(RAN) $(COM)$(LIB)
	$(RM) $^

e_string.o: 
	$(CC) $(CF) $(STD) $(COM)e_string.c -o $(COM)$@

style:
	clang-format -style=google -n *.h *.c $(COM)*.h $(COM)*.c

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) $(COM)*.o 	
	$(RM) $(COM)$(LIB) 

rebuild: clean e_grep

debug: e_grep
	echo ---------------------------------------------------------------
	./e_grep s test_grep/test_0_grep.txt -o
	./e_grep -nivh = test_grep/test_1_grep.txt test_grep/test_2_grep.txt
	./e_grep -e A -ne12345678 text.txt
	./e_grep -ne12345678 text.txt
	./e_grep -e A text.txt -e n
	./e_grep -e A text.txt B
	./e_grep -e-e text.txt -e text2.txt
	./e_grep -e-e -e text.txt
	./e_grep -i -v A
	./e_grep -e text.txt o -i fgdf -ne -e
	./e_grep -e text.txt o -i fgdf -nen -e
	./e_grep -e text.txt o -i fgdf -nene -e
	./e_grep -e A text.txt
	./e_grep -e A text.txt -e
	./e_grep -o -e ina -e rina -o  test.txt
	./e_grep -o -e rina -e ina -o  test.txt
	./e_grep -e A -ne12345678 text.txt
	./e_grep -ne12345678 text.txt
	./e_grep -e A text.txt -e n
	./e_grep -e A text.txt B
	./e_grep -e-e text.txt -e text2.txt
	./e_grep -e text.txt o -i fgdf -ne -e
	./e_grep -e A text.txt
	./e_grep - text.txt
	./e_grep test text.txt
	./e_grep test text.txt -l
	./e_grep test text.txt -l -en
	./e_grep -f file.txt text.txt
	./e_grep -f file.txt A text.txt
	./e_grep -ef file.txt A text.txt.
	./e_grep -fe file.txt A text.txt
	./e_grep -f file.txt text.txt -ffile.txt -en
	./e_grep -e-e -e text.txt
	./e_grep -i -v A
	./e_grep -e text.txt o -i fgdf -nen -e
	./e_grep -e text.txt o -i fgdf -nene -e
	./e_grep -e A text.txt -e
	./e_grep -nlkjhgedf text.txt
	./e_grep test text.txt -
	./e_grep - text.txt -en
	./e_grep - text.txt
	./e_grep -c -e . test_1_grep.txt -e '.'

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
