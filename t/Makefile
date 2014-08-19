# the tap.c submodule must be checked out to build and run tests

CFLAGS += -Wall -Wextra -g -I..

TESTS = 01-sanity.t 10-basic.t 10-lookup.t

01-sanity.t: CFLAGS += -std=c99 -pedantic -Werror

%.t: %.c ../mini.c ../mini.h tap.c/tap.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: tests
	prove .

tests: $(TESTS)

all: tests

clean:
	$(RM) $(TESTS)

.PHONY: all clean check tests
