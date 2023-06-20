# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                           #
#    src/grep/Makefile для проекта SimpleBashUtilits-1: s21_grep            #
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
SRC = main.c s21_grep.c
OBJ = main.o s21_grep.o
LIB = s21_string.a
EXE = s21_grep


s21_grep: $(OBJ) $(LIB)
	$(CC) $(OBJ) -o $(EXE) -L$(COM) $(COM)$(LIB)
	$(RM) *.o

main.o:
	$(CC) $(CF) $(DEB) main.c $(STD)

s21_grep.o:
	$(CC) $(CF) $(DEB) s21_grep.c $(STD) 

$(LIB): s21_string.o
	$(AR) $(COM)$(LIB) $(COM)$^
	$(RAN) $(COM)$(LIB)
	$(RM) $^

s21_string.o: 
	$(CC) $(CF) $(STD) $(COM)s21_string.c -o $(COM)$@

style:
	clang-format -i *.h *.c $(COM)*.h $(COM)*.c

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) $(COM)*.o 	
	$(RM) $(COM)$(LIB) 

rebuild: clean s21_grep

debug: s21_grep
	echo ---------------------------------------------------------------
	./s21_grep s test_grep/test_0_grep.txt -o
	./s21_grep -nivh = test_grep/test_1_grep.txt test_grep/test_2_grep.txt
	./s21_grep -e A -ne12345678 text.txt
	./s21_grep -ne12345678 text.txt
	./s21_grep -e A text.txt -e n
	./s21_grep -e A text.txt B
	./s21_grep -e-e text.txt -e text2.txt
	./s21_grep -e-e -e text.txt
	./s21_grep -i -v A
	./s21_grep -e text.txt o -i fgdf -ne -e
	./s21_grep -e text.txt o -i fgdf -nen -e
	./s21_grep -e text.txt o -i fgdf -nene -e
	./s21_grep -e A text.txt
	./s21_grep -e A text.txt -e
	./s21_grep -o -e ina -e rina -o  test.txt
	./s21_grep -o -e rina -e ina -o  test.txt
	./s21_grep -e A -ne12345678 text.txt
	./s21_grep -ne12345678 text.txt
	./s21_grep -e A text.txt -e n
	./s21_grep -e A text.txt B
	./s21_grep -e-e text.txt -e text2.txt
	./s21_grep -e text.txt o -i fgdf -ne -e
	./s21_grep -e A text.txt
	./s21_grep - text.txt
	./s21_grep test text.txt
	./s21_grep test text.txt -l
	./s21_grep test text.txt -l -en
	./s21_grep -f file.txt text.txt
	./s21_grep -f file.txt A text.txt
	./s21_grep -ef file.txt A text.txt.
	./s21_grep -fe file.txt A text.txt
	./s21_grep -f file.txt text.txt -ffile.txt -en
	./s21_grep -e-e -e text.txt
	./s21_grep -i -v A
	./s21_grep -e text.txt o -i fgdf -nen -e
	./s21_grep -e text.txt o -i fgdf -nene -e
	./s21_grep -e A text.txt -e
	./s21_grep -nlkjhgedf text.txt
	./s21_grep test text.txt -
	./s21_grep - text.txt -en
	./s21_grep - text.txt
	./s21_grep -c -e . test_1_grep.txt -e '.'

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
