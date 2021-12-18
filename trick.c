#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int munch(int vx, int vy, int xmin, int ymin, int xmax, int ymax) {
    int x = 0, y = 0;
    int i = 0;

    while (y > ymin) {
        x += vx; y += vy;
        if (vx >= 1) vx--;
        else if (vx <= -1) vx++;
        vy -= 1;

        if ((xmin <= x) && (x <= xmax) && (ymin <= y) && (y <= ymax)) {
            //printf("solved at x=%d y=%d (step %d)\n", x, y, i);
            return 1;
        }
        i++;
    }

    return 0;
}

int abs(int x) { return (x > 0) ? x : -x; }

int main(int argc, char **argv) {

    int xmin, xmax, ymin, ymax;
    if (argc == 5) {
        xmin = atoi(argv[1]);
        xmax = atoi(argv[2]);
        ymin = atoi(argv[3]);
        ymax = atoi(argv[4]);
    } else if (argc != 1) {
        fprintf(stderr, "usage: %s XMIN XMAX YMIN YMAX\n", argv[0]);
        exit(1);
    } else {
        if (scanf("target area: x=%d..%d, y=%d..%d\n", &xmin, &xmax, &ymin, &ymax) != 4) {
            fprintf(stderr, "invalid input!\n");
            exit(1);
        }
    }

    // part 1.
    // Note that there is a closed form solution to this problem.
    // In this case, we can just choose some initial x velocity that
    // "barely" gets us to the allowed box. In particular, due to the
    // "drag" on the probe, we have the following relation.
    //
    //      |max x attained| = 1 + 2 + 3 + ... + |initial x velocity|
    //
    // In the first step, we will move |initial x velocity| away from
    // the origin. The next step, we will move |initial x velocity| - 1,
    // and so on. Recall that this summation has a closed form formula
    //
    //      |max x attained| = |initial x velocity|
    //                          * (|initial x velocity| + 1) / 2,
    //
    // whence a suitable initial x velocity might be found.
    // If no value of initial x velocity works, it is actually impossible
    // to enter the box!
    //
    // Now that we have a suitable x velocity (or rather, confirmed that
    // one exists), we can find a suitable initial y velocity. Note that 
    // we don't actually need to *know* the initial x velocity, just that
    // one exists.
    //
    // For this problem, consider what happens when we launch the probe
    // into the air with a positive y velocity = vy. The probe will go up
    // for exactly vy time steps, stop for one time step, and then go down
    // for exactly vy time steps, and will have height y=0 at t=vy*2 + 1.
    // As before, this means the maximum height of the probe will be
    // 
    //      max height = 1 + 2 + ... + vy
    //                 = vy**2 / 2 + vy.
    //
    // At this point, note that if time steps were not discrete, we could
    // have any initial y velocity that we wanted. However, because time
    // steps are discrete, we will always end up having height exactly
    // y = 0 at t = vy*2 + 1. That means that performing one additional
    // step gets us to y = -(vy + 1). Now, assuming that the allowed
    // box is below y = 0, we will need -(vy + 1) to be in that box.
    // In particular, we can choose -(vy + 1) to be the bottom edge of the
    // box, so we have
    //
    //      ymin = -(vy + 1)
    // 
    // whence
    //
    //      vy = -ymin - 1.
    //
    // whence
    // 
    //      max height = (-ymin - 1)**2 / 2 + (-ymin - 1) / 2.
    //
    printf("part 1 max height = %d\n", ((-ymin - 1) * (-ymin - 1) + (-ymin - 1)) / 2);

    // part 2.
    // For this part, we can constrain the search by keeping in mind that
    // (1) moving in the opposite x-direction will not help
    // (2) moving so fast in the x- or y- directions that we skip the box
    //     will not help

    int n = 0;
    for (int vx = 0; abs(vx) <= abs(xmax); vx += xmax / abs(xmax)) {
        for (int vy = ymin; vy <= -ymin; vy++) {
            int r = munch(vx, vy, xmin, ymin, xmax, ymax);
            if (!r) continue;
            //printf("%d,%d works :)\n", vx, vy);
            n++;
        }
    }

    printf("part 2 found %d possibilities\n", n);
}
