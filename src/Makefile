CC ?= gcc
CFLAGS ?= -Wall -Werror -Wextra -std=c11
GCOV_FLAGS?=-fprofile-arcs -ftest-coverage
LINUX_LIBS=-lcheck -lm -lpthread -lrt -lsubunit
OS=$(shell uname -s)
green=\033[0;32m
reset=\033[0m

test: s21_string.a
	@$(CC) $(CFLAGS) -c tests/test_*.c main.c
	@mv *.o ./libs/
ifeq ($(OS), Darwin)
	@$(CC) $(CFLAGS) -lcheck ./libs/test_*.o ./libs/main.o s21_string.a -o test
else
	@$(CC) $(CFLAGS) ./libs/test_*.o ./libs/main.o s21_string.a -o test $(LINUX_LIBS)
endif
	@./test

gcov_report: clean test_gcov
	@./test_gcov.out >> /dev/null
	lcov -t "" -o coverage.info --no-external -c -d ./
	#@genhtml -q coverage.info -o ./
	#@mv *.gcda *.gcno ./libs/
	#@open ./index.html
	#@echo "${green}Coverage report is in ./index.html${reset}"

s21_string.a:
	@$(CC) $(CFLAGS) -c libs/s21_*.c
	@mv *.o ./libs/
	@ar rcs $@ ./libs/*.o
	@ranlib $@

test_gcov: s21_string.a
ifeq ($(OS), Darwin)
	@$(CC) $(GCOV_FLAGS) $(CFLAGS) main.c libs/s21_*.c tests/test_*.c s21_string.a -lcheck -o test_gcov.out
else
	@$(CC) $(CFLAGS) main.c libs/s21_*.c tests/test_*.c s21_string.a -o test_gcov.out $(LINUX_LIBS) $(GCOV_FLAGS)
endif

clean:
	@rm -rf ./libs/*.o *.a test *.png ./libs/*.gcda ./libs/*.gcno *.html *.info *.css *.out cmd_line ./src/tests/*.html ./src/*.html ./src/tests ./src
	@echo "${green}Successfully cleaned${reset}"

.PHONY: clean test s21_string.a gcov_report
