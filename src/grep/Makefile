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
SRC = e_grep.c
OBJ = e_grep.o
LIB = String
EXE = e_grep
STD = -std=c11 -pedantic


e_grep: e_grep.o
	$(CC) -o $(EXE) $(OBJ) -L$(COM) -l$(LIB)
	rm *.o

#test:
#	sh test_grep/

e_grep.o:
	$(CC) $(CF) e_grep.c $(STD)

lib: $(COM)e_string.o
	ar rv $(COM)libString.a $^
	rm $^

$(COM)e_string.o: 
	$(CC) $(CF) $(COM)e_string.c -o $@ $(STD)

#grep_optdef.o: 
#	$(CC) $(CF) -o grep_optdef.c $@

clean:
	rm -rf $(EXE) $(OBJ) $(COM)libString.a

rebuild: clean e_grep

test:
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



# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				 																																	  #
#    Справка по созданию библиотеки																					#
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
