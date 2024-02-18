SHELL := /bin/bash
FLAG =-Wall -Wextra -Werror
CHECK =valgrind --tool=memcheck --leak-check=yes
FILE=test.txt

all: clean s21_grep tests

style:
	clang-format -style=Google -i *.c
s21_grep:
	gcc $(FLAG) s21_grep.c -o s21_grep

clean:
	rm -f s21_grep
	rm -f .clang-format

tests:
	python3 autotest.py
