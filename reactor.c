#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PART2
#define PART2 1
#endif
// use -DPART2=0 for part 1.

struct box {
    long long x0, x1, y0, y1, z0, z1;
    char on;
};

int min(int x, int y) { return (x < y) ? x : y; }
int max(int x, int y) { return (x > y) ? x : y; }
int abs(int x) { return (x > 0) ? x : -x; }

struct box intersect(struct box *a, struct box *b) {
    struct box c;
    c.x0 = max(a->x0, b->x0);
    c.x1 = min(a->x1, b->x1);
    c.y0 = max(a->y0, b->y0);
    c.y1 = min(a->y1, b->y1);
    c.z0 = max(a->z0, b->z0);
    c.z1 = min(a->z1, b->z1);

    return c;
}

long long vol(struct box a) {
    if ((a.x0 > a.x1) || (a.y0 > a.y1) || (a.z0 > a.z1)) return 0;
    return (a.x1 - a.x0 + 1) * (a.y1 - a.y0 + 1) * (a.z1 - a.z0 + 1);
}

char is_small(struct box a) {
    return (abs(a.x0) <= 50) && (abs(a.x1) <= 50) && (abs(a.y0) <= 50) && (abs(a.y1) <= 50) && (abs(a.z0) <= 50) && (abs(a.z1) <= 50);
}

int main(int argc, char **argv) {
    struct box boxes[65536];
    int nboxes = 0;

    char buf[8];
    struct box b;
    long long volume = 0;
    while (scanf("%3s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld\n", buf, &b.x0, &b.x1, &b.y0, &b.y1, &b.z0, &b.z1) == 7) {
        int next_nboxes = nboxes;
        if (buf[1] == 'n') b.on = 1;
        else b.on = 0;

#if !PART2
        if (!is_small(b)) continue;
#endif

        // for each new box, we will immediately add its volume if this is an
        // "on" box. We will account for other intersecting boxes later.
        volume += (b.on) ? vol(b) : 0;
        for (int i = 0; i < nboxes; i++) {
            // For each past box, consider the intersection of the current
            // box with the past box.
            struct box intersection = intersect(&boxes[i], &b);

            // If there is no intersection, don't care.
            if (vol(intersection) == 0) continue;

            // What we do with the intersection actually depends on the function
            // of the previous box. If it was an "on"-box, then no matter
            // whether the current box is a "on" or "off" box, we will turn off
            // the intersection. Otherwise, we will turn it back on.
            // (It may seem odd to turn on overlapping "off" boxes, but note
            // that we only save intersections for "off" boxes, never the boxes
            // themselves! So if we actually had an overlapping "off" box, that
            // means that there is an overlapping "on" box underlying it that
            // we already counted.)
            intersection.on = !boxes[i].on;
            if (boxes[i].on) volume -= vol(intersection);
            else volume += vol(intersection);

            // For all intersections, we will save them for future box
            // processing.
            boxes[next_nboxes++] = intersection;
        }

        // For "on" boxes only, we will save them for future processing.
        // Why not off boxes? The non-intersecting parts of "off boxes" have no
        // use at all, so we will discard them. They will also cause confusion
        // for the assumption in parentheses in the above for loop.
        if (b.on) boxes[next_nboxes++] = b;

        nboxes = next_nboxes;
    }

    printf("volume = %lld\n", volume);
}
