#!/bin/bash

./s21_grep e s21_grep.c > s21_grep.txt
grep e s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -e "for" s21_grep.c > s21_grep.txt
grep -e "for" s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt
./s21_grep -e "for" -e int s21_grep.c > s21_grep.txt
grep -e "for" -e int s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -i All grep_test.txt > s21_grep.txt
grep -i All grep_test.txt > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -v e s21_grep.c > s21_grep.txt
grep -v e s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -c e s21_grep.c grep_test.txt > s21_grep.txt
grep -c e s21_grep.c grep_test.txt > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -c -v e s21_grep.c > s21_grep.txt
grep -c -v e s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -l e s21_grep.c s21_grep.c s21_grep.c > s21_grep.txt
grep -l e s21_grep.c s21_grep.c s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -n e s21_grep.c > s21_grep.txt
grep -n e s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -h e s21_grep.c grep_test.txt > s21_grep.txt
grep -h e s21_grep.c grep_test.txt > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -s e s21_grp.c > s21_grep.txt
grep -s e s21_grp.c > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt

./s21_grep -f grep_pattern.txt grep_test.txt -n > s21_grep.txt
grep -f grep_pattern.txt grep_test.txt -n > grep.txt
diff -s s21_grep.txt grep.txt
rm s21_grep.txt grep.txt