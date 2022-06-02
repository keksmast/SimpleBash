CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
SOURS = s21_grep.c
EX = -o s21_grep

all: make_test

s21_grep:
	$(CC) $(CFLAGS) $(SOURS) $(EX)

make_test: s21_grep
	sh grep_tests.sh

clean:
	rm -rf s21_grep