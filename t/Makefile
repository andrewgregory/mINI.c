CFLAGS += -Wall -g -I..

OBJECTS = \
		  10-basic.t \
		  20-dynamic.t

%.t: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

all: $(OBJECTS)

check: all
	PATH=.:$$PATH prove $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

.PHONY: all clean check
