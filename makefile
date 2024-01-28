CC = gcc
CFLAGS = -Wall -Werror -std=c11 -pedantic


s21_grep: s21_grep.c
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

style: 
	clang-format -style=Google -i *.c
leaks:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./s21_grep - test.txt

clean:
	rm -f s21_grep 