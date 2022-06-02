CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
SOURS = s21_cat.c
EX = -o s21_cat

all: make_test

s21_cat:
	$(CC) $(CFLAGS) $(SOURS) $(EX)

make_test: s21_cat
	sh cat_tests.sh

clean:
	rm -rf s21_cat