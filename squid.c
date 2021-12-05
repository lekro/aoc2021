#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOARDS 128

#ifndef DEBUG
#define DEBUG 0
#endif


void print_boards(int *buf, int board) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d\t", buf[board * 25 + i * 5 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int calc_score(int *boards, int board) {
    int score = 0;
    for (int i = 0; i < 25; i++) {
        int x = boards[board * 25 + i];
        if (x >= 0) score += x;
    }
    return score;
}


int main(int argc, char **argv) {
    char buf[1024];
    int call[128];
    int boards[5 * 5 * MAX_BOARDS];
    int won[MAX_BOARDS];
    int winning_score[MAX_BOARDS];
    const int empty_row[5] = {-1, -1, -1, -1, -1};

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        fprintf(stderr, "invalid input - first line needed!!!\n");
    }
    char *tok = NULL;
    int nnumb = 0;
    while ((tok = strtok((tok == NULL) ? buf : NULL, ",\n")) != NULL) {
        call[nnumb++] = atoi(tok);
    }

    int nboards = 0;
    int bindex = 0;
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        while ((tok = strtok((tok == NULL) ? buf : NULL, " \n")) != NULL) {
            boards[nboards * 25 + bindex++] = atoi(tok);
            //printf("added board=%d index=%d ->  %d\n", nboards, bindex - 1, boards[nboards * 25 + bindex - 1]);
        }

        if (bindex >= 25) {
            bindex = 0;
            won[nboards] = -1;
            nboards++;
        }
    }

    int winner = -1;
    int last_numb = -1;
    int loser = -1;
    for (int i = 0; i < nnumb; i++) {
        last_numb = call[i];
        if (DEBUG) printf("checking: %d\n", last_numb);
        // apply new number
        for (int board = 0; board < nboards; board++) {
            for (int j = 0; j < 25; j++) {
                if (boards[board * 25 + j] == call[i]) boards[board * 25 + j] = -1;
            }
        }

        // check bingo
        for (int board = 0; board < nboards; board++) {
            // rows are easy
            for (int row = 0; row < 5; row++) {
                if (memcmp(&boards[board * 25 + row * 5], empty_row, sizeof(empty_row)) == 0) {
                    if (DEBUG) printf("won via row\n");
                    if (winner == -1) winner = board;
                    if (won[board] == -1) {
                        won[board] = last_numb;
                        winning_score[board] = calc_score(boards, board);
                    }
                }
            }

            // columns aren't
            for (int column = 0; column < 5; column++) {
                int col_ok = 1;
                for (int row = 0; row < 5; row++) {
                    if (boards[board * 25 + row * 5 + column] != -1) {
                        col_ok = 0;
                        break;
                    }
                }

                if (col_ok) {
                    if (DEBUG) printf("won via column\n");
                    if (winner == -1) winner = board;
                    if (won[board] == -1) {
                        won[board] = last_numb;
                        winning_score[board] = calc_score(boards, board);
                    }
                }
            }

        }


        int nwon = 0;
        int last_zero = -1;
        for (int board = 0; board < nboards; board++) {
            nwon += (won[board] >= 0) ? 1 : 0;
            if (won[board] == -1) last_zero = board;
        }

        if (nwon == nboards - 1) {
            loser = last_zero;
            if (DEBUG) printf("last winner = %d\n", last_zero);
        }


    }
print_winner:
    printf("winner = %d score = %d * %d = %d\n", winner, winning_score[winner], won[winner], winning_score[winner] * won[winner]);
    printf("loser = %d score = %d * %d = %d\n", loser, winning_score[loser], won[loser], winning_score[loser] * won[loser]);
    return 0;
}
