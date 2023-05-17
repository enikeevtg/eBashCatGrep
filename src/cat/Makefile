CC = gcc
CF = -Wall -Werror -Wextra

e_cat :
	$(CC) $(CF) e_cat.c -o e_cat

e_cat_test :
	$(CC) $(CF) e_cat.c -o test_cat/e_cat

clean :
	rm -rf e_cat

rebuild :
	rm -rf e_cat
	$(CC) $(CF) e_cat.c -o e_cat
