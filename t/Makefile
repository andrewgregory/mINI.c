CFLAGS += -Wall -g -I..

OBJECTS = 10-basic.t

%.t: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

all: $(OBJECTS)

check: all
	PATH=.:$$PATH LD_LIBRARY_PATH=../lib prove $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

.PHONY: all clean check
