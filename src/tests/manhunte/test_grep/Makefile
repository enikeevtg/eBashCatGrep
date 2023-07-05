CC=gcc
CFLAGS=-c -Wall -Wextra -Werror

e_grep: main_grep func_grep
	$(CC) e_grep.o func_grep.o -o e_grep

main_grep:
	$(CC) $(CFLAGS) e_grep.c

func_grep:
	$(CC) $(CFLAGS) ../common/func_grep.c

test:

rebuild: clean e_grep

clean:
	rm -rf *.o e_grep