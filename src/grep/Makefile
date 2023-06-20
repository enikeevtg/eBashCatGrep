# utilities
CC = gcc
AR = ar rv
RAN = ranlib
RM = rm -rf

# utilities options
CF = -Wall -Werror -Wextra -c
STD = -std=c11 -pedantic

# filenames
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

test:
	sh test_grep/test_func_grep.sh

leak:
	sh test_grep/test_leak_grep.sh

# linux VM needed!
valgrind:
	bash test_grep/test_ERROR_VALGRIND.sh

# service
style:
	clang-format -style=google -n *.h *.c $(COM)*.h $(COM)*.c

tostyle:
	clang-format -style=google -i *.h *.c $(COM)*.h $(COM)*.c

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) *.log

fclean: clean
	$(RM) $(COM)*.o
	$(RM) $(COM)$(LIB)

rebuild: clean e_grep
