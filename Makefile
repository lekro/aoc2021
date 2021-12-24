SOURCES = depth.c hello.c updown.c updown2.c power.c squid.c vents.c lanternfish.c crabs.c digits.c tubes.c nav.c octopus.c caves.c origami.c polymer.c chiton.c packet.c trick.c snailfish.c scanner.c trench.c dice.c reactor.c amphipod.c
CC = gcc
CFLAGS = -g -O3 -fstack-protector #-DDEBUG
LDFLAGS = -lm
TARGETS = $(SOURCES:.c=)

all: $(TARGETS)

$(TARGETS): %: %.c

clean:
	rm -f $(TARGETS)
