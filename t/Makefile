CFLAGS += -Wall -g -I..

TESTS = 10-basic.t

%.t: %.c ../mini.c ../mini.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: all
	PATH=.:$$PATH prove $(TESTS)

tests: $(TESTS)

all: tests

clean:
	$(RM) $(TESTS)

.PHONY: all clean check tests
