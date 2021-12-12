SOURCES = depth.c hello.c updown.c updown2.c power.c squid.c vents.c lanternfish.c crabs.c digits.c tubes.c nav.c octopus.c caves.c
CC = gcc
CFLAGS = -g -Og -fstack-protector
LDFLAGS = -lm
TARGETS = $(SOURCES:.c=)

all: $(TARGETS)

$(TARGETS): %: %.c

clean:
	rm -f $(TARGETS)
