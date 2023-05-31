# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#  																																					#
#    Makefile для проекта SimpleBashUtilits-1: e_grep                       #
#    (c) T. Enikeev                                                         #
#    zeftyrst@student.21-school.ru                                          #
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
	cp e_grep test_grep/

test:
	cp e_grep test_grep/
	sh test_grep/test_func_grep.sh

main.o:
	$(CC) $(CF) main.c $(STD)

e_grep.o:
	$(CC) $(CF) e_grep.c $(STD)

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


#-----------------debugging--------------------
e_grep_deb: main_deb.o e_grep_deb.o lib
	$(CC) -o $(EXE) $(OBJ) -L$(COM) -l$(LIB)
	rm *.o

main_deb.o:
	$(CC) $(CF) main.c $(STD) $(DEB)

e_grep_deb.o:
	$(CC) $(CF) e_grep.c $(STD) $(DEB)

debug: e_grep_deb
	echo ---------------------------------------------------------------
	./e_grep s test_grep/test_0_grep.txt -o

ngbfdc:
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

