CC = gcc
CF = -c -Wall -Werror -Wextra

e_cat: e_cat.o e_string.o
	$(CC) e_cat.o e_string.o -o e_cat

for_test: e_cat.o e_string.o
	$(CC) e_cat.o e_string.o -o test_cat/e_cat

e_cat.o: e_cat.c
	$(CC) $(CF) e_cat.c

e_string.o: ../common/e_string.c
	$(CC) $(CF) ../common/e_string.c

clean:
	rm -rf e_cat *.o

rebuild: clean e_cat
