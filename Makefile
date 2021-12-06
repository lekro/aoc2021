SOURCES = depth.c hello.c updown.c updown2.c power.c squid.c vents.c lanternfish.c
CC = gcc
CFLAGS = -g -Og
LDFLAGS = -lm
TARGETS = $(SOURCES:.c=)

all: $(TARGETS)

$(TARGETS): %: %.c

clean:
	rm -f $(TARGETS)
