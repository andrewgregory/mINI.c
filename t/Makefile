# the tap.c submodule must be checked out to build and run tests

CFLAGS += -Wall -Wextra -g -I..

TESTS = 01-sanity.t 10-basic.t 10-lookup.t 90-smoke.t

01-sanity.t: CFLAGS += -std=c99 -pedantic -Werror

%.t: %.c ../mini.c ../mini.h tap.c/tap.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: tests
	prove .

gcov: CC = gcc
gcov: CFLAGS += -fprofile-arcs -ftest-coverage
gcov: check
	gcov $(TESTS)

tests: $(TESTS)

all: tests

clean:
	$(RM) $(TESTS)
	$(RM) *.gcov *.gcda *.gcno

.PHONY: all clean check gcov tests
