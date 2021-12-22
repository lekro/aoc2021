#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NPLAYERS 2
#define MAXPOS 2048
int n_die = 0;

long long score1a[MAXPOS*MAXPOS], score1b[MAXPOS*MAXPOS], score2a[MAXPOS*MAXPOS], score2b[MAXPOS*MAXPOS];

int deterministic_die() {
    n_die++;
    static int roll = 1;
    roll %= 100;

    printf("%d ", roll);
    return roll++;
}

int ii(int r, int c) {
    return r * MAXPOS + c;
}


void print_arr(long long *x) {
    for (int r = 0; r < MAXPOS; r++) {
        for (int c = 0; c < MAXPOS; c++) {
            printf("%lld ", x[ii(r, c)]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char **argv) {

    int pos1init, pos2init;

    if (argc == 3) {
        pos1init = atoi(argv[1]);
        pos2init = atoi(argv[2]);
        if (!pos1init || !pos2init) {
            fprintf(stderr, "initial positions of player 1 and 2 need to be between 1-10\n");
            exit(1);
        }
    } else {
        if ((scanf("Player 1 starting position: %d\n", &pos1init) != 1) || (scanf("Player 2 starting position: %d\n", &pos2init) != 1)) {
            fprintf(stderr, "invalid input file\n");
            exit(1);
        }
    }

    if ((pos1init < 1) || (pos2init < 1) || (pos2init > 10) || (pos2init > 10)) {
        fprintf(stderr, "initial positions of player 1 and 2 need to be between 1-10\n");
        exit(1);
    }

    pos1init -= 1; pos2init -= 1;

    // part 1.
    int pos[NPLAYERS];
    int score[NPLAYERS];

    pos[0] = pos1init; pos[1] = pos2init;
    memset(score, 0, sizeof(score));

    while (1) {
        for (int player = 0; player < NPLAYERS; player++) {
            for (int die = 0; die < 3; die++) {
                pos[player] += deterministic_die();
            }
            pos[player] %= 10;
            score[player] += pos[player] + 1;
            printf("-> %d [at %d]\n", score[player], pos[player]+1);
            if (score[player] >= 1000) goto endpart1;
        }

    }

endpart1:
    int min_score = INT_MAX;
    for (int player = 0; player < NPLAYERS; player++) {
        if (score[player] < min_score) min_score = score[player];
    }


    printf("part 1 %d * %d = %d\n", min_score, n_die, min_score * n_die);

    // part 2.
    // THIS IS A COOL PROBLEM.

    // each of these arrays stores frequencies of particular states.
    // note that the process (score) is not Markovian; it depends on the positions.
    // however, (score, pos) is Markovian. But this means that the number of
    // possible states will explode. Let's see what happens :)
    //
    // this is reminiscent of the "coupling argument" used to prove certain
    // things about Markov chains; indeed, we are playing with Markov chains
    // here too.
    //
    // Player 1 and Player 2 play the game essentially independently in these
    // "rounds", i.e. player 2's score is always independent (probabilistically)
    // from player 1. This is useful because we can just pretend player 1 and
    // player 2 live in entirely different super-universes and separate out
    // their actions entirely. So we can count the number of "universes" in which
    // player 1 won by taking each "universe" where player 1 just scored 21 or
    // above in the current time step, and combining it with any of the
    // possible universes where player 2 has not yet scored 21 or above.
    // This "combination" turns out to be multiplication: for each of player
    // 1's universes with score >= 21, there is the same number of player 2's
    // universes with score < 21. We then forget about these instances by
    // dropping player 1's universes with score >= 21, then computing
    // player 2's universes, and moving to the next time step.

    // rows of score matrices indicate the current position;
    // columns indicate the score. the number in the matrix indicates the
    // frequency, i.e. "number of universes."

    long long win1 = 0;
    long long *score1old = score1a, *score1new = score1b;
    long long *score2old = score2a, *score2new = score2b;
    memset(score1old, 0, sizeof(score1a));
    memset(score2old, 0, sizeof(score2a));

    // where do players start???
    score1old[ii(pos1init, 0)] = 1;
    score2old[ii(pos2init, 0)] = 1;

    int round = 1;
    while (round < 100) {
        // Calculate the number of "universes" that have a particular
        // position and score after player 1 rolls the "Dirac dice".
        memset(score1new, 0, sizeof(score1a));
        for (int p = 0; p < MAXPOS - 9; p++) {
            // update scores
            for (int s = 0; s < 21; s++) {

                // For each (position, score), we figure out what happens to it.
                // When we roll three "Dirac dice", we get 27 possibilities.
                // One of these is all ones, and another is all threes.
                // We are interested in the sum, so there's multiple ways to
                // get e.g. 4: 2,1,1 or 1,2,1 or 1,1,2...
                // Fortunately probability class has already taught us about
                // how to get the distribution of the sum of three such dice.
                // In particular...
                // convolve three uniform distributions together :)
                // weights are then [1, 3, 6, 7, 6, 3, 1].
                score1new[ii(p+3, s+(p+3)%10 + 1)] += score1old[ii(p, s)];
                score1new[ii(p+4, s+(p+4)%10 + 1)] += score1old[ii(p, s)] * 3;
                score1new[ii(p+5, s+(p+5)%10 + 1)] += score1old[ii(p, s)] * 6;
                score1new[ii(p+6, s+(p+6)%10 + 1)] += score1old[ii(p, s)] * 7;
                score1new[ii(p+7, s+(p+7)%10 + 1)] += score1old[ii(p, s)] * 6;
                score1new[ii(p+8, s+(p+8)%10 + 1)] += score1old[ii(p, s)] * 3;
                score1new[ii(p+9, s+(p+9)%10 + 1)] += score1old[ii(p, s)];
            }
        }

        // check wins
        long long nwon = 0;
        long long nnotwon = 0;
        for (int p = 0; p < MAXPOS; p++) {
            for (int s = 0; s < 21; s++) {
                nnotwon += score2old[ii(p, s)];
            }
            for (int s = 21; s < MAXPOS; s++) {
                nwon += score1new[ii(p, s)];
                score1new[ii(p, s)] = 0;
                // we set it to zero because these "universes" already won.
                // aint no use double counting em
            }
        }

        // essentially compare pairwise "won" and "not won" in this round
        win1 += nwon * nnotwon;

        // player 2: same idea as updating p1 state.
        memset(score2new, 0, sizeof(score2a));
        for (int p = 0; p < MAXPOS - 9; p++) {
            // update frequencies

            // update scores
            for (int s = 0; s < MAXPOS - 21; s++) {
                // convolve three uniform dist. together :)
                // weights are then [1, 3, 6, 7, 6, 3, 1].
                score2new[ii(p+3, s+(p+3)%10 + 1)] += score2old[ii(p, s)];
                score2new[ii(p+4, s+(p+4)%10 + 1)] += score2old[ii(p, s)] * 3;
                score2new[ii(p+5, s+(p+5)%10 + 1)] += score2old[ii(p, s)] * 6;
                score2new[ii(p+6, s+(p+6)%10 + 1)] += score2old[ii(p, s)] * 7;
                score2new[ii(p+7, s+(p+7)%10 + 1)] += score2old[ii(p, s)] * 6;
                score2new[ii(p+8, s+(p+8)%10 + 1)] += score2old[ii(p, s)] * 3;
                score2new[ii(p+9, s+(p+9)%10 + 1)] += score2old[ii(p, s)];
            }
        }

        if (!nnotwon) break;
        // when we have nobody "not yet winning" in player 2, we quit since
        // player 1 can never win again.

        printf("in round %d: %lld won and %lld not won; i.e. %lld wins\n", round++, nwon, nnotwon, win1);


        // swap buffers
        long long *tmp = score1new;
        score1new = score1old;
        score1old = tmp;
        tmp = score2new;
        score2new = score2old;
        score2old = tmp;
    }

    printf("part 2 %lld\n", win1);

}
