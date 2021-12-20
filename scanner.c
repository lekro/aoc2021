#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Note to the reader.
// This problem is a mess
// Please start with the main function, on which some commentary is provided.

#define MAX_BEACONS (3*64)
#define MAX_SCANNERS 32
int beacons[3 * MAX_BEACONS * MAX_SCANNERS];
int nbeacons[MAX_SCANNERS];
int nscanners = 0;

void permute(int *dst, const int *src, int perm) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    // facing direction
    switch (perm >> 2) {
        case 0: // (x, y, z); do nothing
            break;
        case 1: // (z, y, -x)
            dst[0] = src[2];
            dst[2] = -src[0];
            break;
        case 4: // (y, -x, z)
            dst[0] = src[1];
            dst[1] = -src[0];
            break;
        case 3: // (-z, y, x)
            dst[0] = -src[2];
            dst[2] = src[0];
            break;
        case 2: // (-x, y, -z)
            dst[0] = -src[0];
            dst[2] = -src[2];
            break;
        case 5: // (-y, x, z)
            dst[0] = -src[1];
            dst[1] = src[0];
            break;
        default:
            fprintf(stderr, "INVALID PERMUTATION 0x%x", perm);
            exit(1);
    }

    // rotation. y <= -z; z <= y;
    for (int rot = 0; rot < (perm & 0x3); rot++) {
        int tmp = dst[2];
        dst[2] = dst[1];
        dst[1] = -tmp;
    }
}

int perm_compose(int p1, int p2) {
    int dst[3], test[3], src[3] = {1, 2, 3};
    permute(test, src, p1);
    permute(dst, test, p2);

#ifdef DEBUG
    printf("(%d,%d,%d) --(%d)--> (%d,%d,%d) --(%d)--> (%d,%d,%d)\n",
            src[0], src[1], src[2], p1,
            test[0], test[1], test[2], p2,
            dst[0], dst[1], dst[2]);
#endif

    // check which of the permutation matches
    for (int p3 = 0; p3 < 24; p3++) {
        permute(test, src, p3);
        for (int i = 0; i < 3; i++) if (dst[i] != test[i]) goto perm_failed;
#ifdef DEBUG
        printf("(%d,%d,%d) --(%d)--> (%d,%d,%d)\n",
                src[0], src[1], src[2], p3,
                test[0], test[1], test[2]);
#endif
        return p3;
perm_failed:
    }
    assert(0);

    return -1;
}

int perm_invert(int p1) {
    int dst[3], test[3], src[3] = {1, 2, 3};
    permute(dst, src, p1);

#ifdef DEBUG
    printf("(%d,%d,%d) --(%d)--> (%d,%d,%d)\n",
            src[0], src[1], src[2], p1,
            dst[0], dst[1], dst[2]);
#endif

    // check which of the permutation matches
    for (int p3 = 0; p3 < 24; p3++) {
        permute(test, dst, p3);
        for (int i = 0; i < 3; i++) if (test[i] != src[i]) goto perm_failed;
#ifdef DEBUG
        printf("(%d,%d,%d) --(%d)--> (%d,%d,%d)\n",
                dst[0], dst[1], dst[2], p3,
                test[0], test[1], test[2]);
#endif
        return p3;
perm_failed:
    }
    assert(0);

    return -1;
}

#define MAXPOP 1024
int pop[3 * MAXPOP];
int npop[MAXPOP];

int try_pair(int *off, int scan1, int scan2, int perm) {

    memset(npop, 0, sizeof(npop));

    for (int b1 = 0; b1 < nbeacons[scan1]; b1++) {
        for (int b2 = 0; b2 < nbeacons[scan2]; b2++) {

            // permute
            int dst[3];
            permute(dst, &beacons[3 * b2 + scan2 * MAX_BEACONS], perm);

            // subtract
            int *b1i, *b2i;
            b1i = &beacons[3 * b1 + scan1 * MAX_BEACONS];
            b2i = dst;
            for (int i = 0; i < 3; i++) b2i[i] -= b1i[i];

            // compare
            for (int i = 0; i < MAXPOP; i++) {
                assert(i != MAXPOP - 1);
                // check if we are at the end of pop
                if (npop[i] == 0) {
                    // not seen this before
                    for (int j = 0; j < 3; j++) pop[3*i+j] = b2i[j];
                    npop[i] = 1;
                    break;
                }

                // check if the difference didn't match
                for (int j = 0; j < 3; j++) if (pop[3*i+j] != b2i[j]) goto nomatch;

                // difference matched; increment npop
                npop[i]++;
                break;
nomatch:
            }

        }
    }

    // find max of npop
    int popmax = 0, popidx = -1;
    for (int i = 0; (i < MAXPOP) && (npop[i] != 0); i++) {
        if (npop[i] > popmax) {
            popmax = npop[i];
            popidx = i;
        }
    }
    assert(popidx != -1);

    // copy offset required
    memcpy(off, &pop[popidx*3], 3*sizeof(*off));
    return popmax;
}

struct offperm {
    int off[3];
    int perm;
};

int abs(int x) {
    return (x > 0) ? x : -x;
}

// ||x - y||_1
int ma(int *x, int *y) {
    int d = 0;
    for (int i = 0; i < 3; i++) {
        d += abs(x[i] - y[i]);
    }

    return d;
}


int all_zero(const int *x) {
    return (!x[0]) && (!x[1]) && (!x[2]);
}


char visited[MAX_SCANNERS];
struct offperm resolve_offsets(const int *offsets, const int *permutations, int scan, int nscanners) {
    struct offperm ret;
    ret.perm = -1;

    if (scan == 0) {
        memset(&ret, 0, sizeof(ret));
        return ret;
    }

    for (int other = 0; other < nscanners; other++) {
        if (all_zero(&offsets[other*MAX_SCANNERS*3 + scan*3])) continue;
        if (visited[other]) continue;
        visited[other] = 1;
        ret = resolve_offsets(offsets, permutations, other, nscanners);
        visited[other] = 0;
        if (ret.perm == -1) continue; // didn't find a path

        // found a path!!!!!!!
        int tmp[3];
        permute(tmp, &offsets[other*MAX_SCANNERS*3 + scan*3], ret.perm);
        ret.perm = perm_compose(permutations[other*MAX_SCANNERS + scan], ret.perm);
        //permute(tmp, ret.off, perm_invert(ret.perm));
        //memcpy(ret.off, tmp, sizeof(tmp));
        for (int j = 0; j < 3; j++) ret.off[j] -= tmp[j];//offsets[other*MAX_SCANNERS*3 + scan*3 + j];
        return ret;
    }

    return ret;
}

int main(int argc, char **argv) {


    memset(nbeacons, 0, sizeof(nbeacons));

    char buf[1024];
    while (fgets(buf, 1024, stdin) != NULL) {
        if (buf[1] == '-') {
            nscanners++;
            continue;
        }

        if (!isdigit(buf[0]) && buf[0] != '-') continue;

        // read numbers
        int bidx = nbeacons[nscanners-1]++;
        int *b = &beacons[3 * bidx + (nscanners-1) * MAX_BEACONS];
        sscanf(buf, "%d,%d,%d\n", b, b+1, b+2);

    }
#ifdef DEBUG
    for (int scan1 = 0; scan1 < nscanners; scan1++) {
        printf("read scanner %d with %d beacons seen:\n", scan1, nbeacons[scan1]);
        for (int b1 = 0; b1 < nbeacons[scan1]; b1++) {
            int *b = &beacons[3 * b1 + scan1 * MAX_BEACONS];
            printf("beacon %d-%d = (%d,%d,%d)\n", scan1, b1, b[0], b[1], b[2]);
        }
    }

    printf("test rotations of (123)...\n");
    int tdst[3], tsrc[3] = {1, 2, 3};
    for (int perm = 0; perm < 24; perm++) {
        permute(tdst, tsrc, perm);
        printf("(%d,%d,%d) --(%d)--> (%d,%d,%d)\n", tsrc[0], tsrc[1], tsrc[2], perm, tdst[0], tdst[1], tdst[2]);
    }
#endif

    // let's determine offsets of scanners and permutation they require
    // rows are source, columns are dest. 
    // permutations(r, c) indicates permutation needed on
    // c to get it to look like r.
    //
    // Theory of operation:
    // For each pair of (scan1, scan2), try to rotate scan2's known beacons
    // for each possible rotation. ("permutation" since I didn't want to change
    // old variable name)
    //      We have (scan1, scan2, rotation), unrotated beacons from scan1,
    //      rotated beacons from scan2. Now subtract all scan2 beacons b2 from
    //      all scan1 beacons b1, pairwise. Differences will also be vectors
    //      in Z^3. Now, find and count unique differences. If this triple
    //      (scan1, scan2, rotation) satisfies (1) scan2 adjacent to scan1,
    //      and (2) rotating scan2 by rotation orients it the same as scan1,
    //      then we should see at least 12 counts on the most common difference.
    //      If this is satisfied, then we write the offset of scan2 relative to
    //      scan1 (and the permutation) in the matrices "offsets" and
    //      "permutations."
    //
    int offsets[MAX_SCANNERS*3 * MAX_SCANNERS*3];
    int permutations[MAX_SCANNERS*MAX_SCANNERS];
    memset(offsets, 0, sizeof(offsets));
    memset(permutations, 0, sizeof(permutations));

    for (int scan1 = 0; scan1 < nscanners; scan1++) {
        for (int scan2 = 0; scan2 < nscanners; scan2++) {
            // consider pair (scan1, scan2).
            if (scan1 == scan2) continue;

            // consider all permutations of scan2.
            for (int perm = 0; perm < 24; perm++) {
#ifdef DEBUG
                printf("(%d, %d) trying perm 0x%x...\n", scan1, scan2, perm);
#endif
                // lower 2 bits indicate "roll", top 3 bits indicate "pitch and yaw".
                int off[3];
                int nmatch = try_pair(off, scan1, scan2, perm);
                if (nmatch >= 12) {
                    printf("scanners (%d, %d) matched %d times with permutation 0x%x\n", scan1, scan2, nmatch, perm);
                    // save offsets and permutations
                    memcpy(&offsets[scan1*MAX_SCANNERS*3 + scan2*3], off, sizeof(off));
                    permutations[scan1*MAX_SCANNERS + scan2] = perm;
                    goto scan2_done;
                }
            }
scan2_done:
        }

    }

    // At this point, we essentially have a (hopefully connected) graph with
    // nodes representing scanners and edges representing offsets and
    // rotations. We need to find paths from every scanner back to scanner 0,
    // such that we can find the offset and rotation of each scanner relative
    // to scanner 0.
    // 
    // now, for every scanner, we need to find a path back to 0. 
    // this sounds like a job for our friend, the recursive function...
    int final_offsets[MAX_SCANNERS * 3];
    int final_perm[MAX_SCANNERS];

    memset(final_offsets, 0, sizeof(final_offsets));
    memset(final_perm, 0, sizeof(final_perm));

    for (int scan = 1; scan < nscanners; scan++) {
        memset(visited, 0, sizeof(visited));
        struct offperm r;
        r = resolve_offsets(offsets, permutations, scan, nscanners);
        printf("scanner %d is at %d,%d,%d, perm 0x%x\n", scan, r.off[0], r.off[1], r.off[2], r.perm);
        memcpy(&final_offsets[scan*3], r.off, sizeof(r.off));
        final_perm[scan] = r.perm;
    }

    // We can now rotate and subtract offsets from measurements taken
    // by the scanners.
    // ok cool now that we have that (finally)
    for (int scan = 0; scan < nscanners; scan++) {
        // permute and subtract offset :)
        for (int beacon = 0; beacon < nbeacons[scan]; beacon++) {
            int tmp[3];
            permute(tmp, &beacons[scan*MAX_BEACONS+beacon*3], final_perm[scan]);
            for (int j = 0; j < 3; j++) tmp[j] += final_offsets[scan*3 + j];
            memcpy(&beacons[scan*MAX_BEACONS+beacon*3], tmp, sizeof(tmp));
#ifdef DEBUG
            fprintf(stderr, "%d,%d,%d\n", tmp[0], tmp[1], tmp[2]);
#endif
        }
    }

    // We can now count unique measurements.
    int nunique = 0;
    for (int scan1 = 0; scan1 < nscanners; scan1++) {
        for (int b1 = 0; b1 < nbeacons[scan1]; b1++) {
            for (int scan2 = 0; scan2 <= scan1; scan2++) {
                for (int b2 = 0; b2 < nbeacons[scan2]; b2++) {
                    if ((scan1 == scan2) && (b1 == b2)) break;
                    for (int j = 0; j < 3; j++) if (beacons[scan1*MAX_BEACONS+b1*3+j] != beacons[scan2*MAX_BEACONS+b2*3+j]) goto unique_ok;
#ifdef DEBUG
                    printf("%d-%d and %d-%d are the same, skipping\n", scan1, b1, scan2, b2);
#endif
                    goto unique_fail;
unique_ok:
                }
            }
            nunique++;
unique_fail:
        }
    }

    printf("part 1 nunique = %d\n", nunique);

    // part 2 manhattan distance
    int maxdist = 0;
    for (int scan1 = 0; scan1 < nscanners; scan1++) {
        for (int scan2 = 0; scan2 < nscanners; scan2++) {
            int *coo1 = &final_offsets[scan1*3];
            int *coo2 = &final_offsets[scan2*3];
            int l1 = ma(coo1, coo2);
            if (l1 > maxdist) maxdist = l1;
        }
    }

    printf("part 2 max l1 = %d\n", maxdist);

}
