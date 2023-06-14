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
SRC = main.c e_cat.c
OBJ = main.o e_cat.o
LIB = e_string.a
EXE = e_cat


e_cat: $(OBJ) $(LIB)
	$(CC) $(OBJ) -o $(EXE) -L$(COM) $(COM)$(LIB)
	$(RM) *.o

main.o:
	$(CC) $(CF) main.c $(STD)

e_cat.o:
	$(CC) $(CF) e_cat.c $(STD)

$(LIB): e_string.o
	$(AR) $(COM)$(LIB) $(COM)$^
	$(RAN) $(COM)$(LIB)
	$(RM) $^

e_string.o: 
	$(CC) $(CF) $(STD) $(COM)e_string.c -o $(COM)$@

rebuild: clean e_cat

test:
	sh test_cat/test_func_cat.sh

leak:
	sh test_cat/test_func_cat.sh

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

