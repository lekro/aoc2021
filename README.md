# aoc2021

Lekro's solutions in C for [aoc2021](https://adventofcode.com/).

Type `make` to compile all programs. Input is accepted from `stdin` for all
programs in this collection, e.g. you may run a program via

```
./depth < depth.input
```

where the input is saved in `depth.input`, or simply type in or paste in
lines of input followed by an EOF (`^D`).

A solution for each puzzle may be posted at least 24 hours after it
opened (with no guarantees on timeliness).

## A mapping from puzzles to solutions

- Day 1: `depth.c`.
- Day 2: `updown.c` (part 1) and `updown2.c` (part 2).
- Day 3: `power.c`.
- Day 4: `squid.c`.
- Day 5: `vents.c`. `-DPART2=0` calculates part 1 (lazy!)
- Day 6: `lanternfish.c`. Change the `NDAYS` macro for part 1 (also lazy!)
- Day 7: `crabs.c`.
- Day 8: `digits.c`.
- Day 9: `tubes.c`.
- Day 10: `nav.c`.
- Day 11: `octopus.c`.
- Day 12: `caves.c`.
- Day 13: `origami.c`.
- Day 14: `polymer.c`. Run part 1 with `./polymer 10` and part 2 with `./polymer 40`.
- Day 15: `chiton.c`. Run part 1 with `./chiton 1` and part 2 with `./chiton 5`.
- Day 16: `packet.c`.
- Day 17: `trick.c`. This one accepts either command line arguments or stdin.
- Day 18: `snailfish.c. Use `-DDEBUG` to follow the explode/split operations.
