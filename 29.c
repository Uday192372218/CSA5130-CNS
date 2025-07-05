#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_LANES 25
#define RATE_LANES 16
#define CAPACITY_LANES 9

int main() {
    int lanes[TOTAL_LANES] = {0};  // 0 = zero lane, 1 = non-zero lane
    int i, rounds = 0;

    srand(time(NULL));

    for (i = 0; i < RATE_LANES; i++)
        lanes[i] = 1;  // Initialize first 16 lanes with data (non-zero)

    while (1) {
        int changed = 0;
        for (i = RATE_LANES; i < TOTAL_LANES; i++) {
            if (lanes[i] == 0) {
                int random_lane = rand() % RATE_LANES;
                if (lanes[random_lane] == 1) {
                    lanes[i] = 1;
                    changed = 1;
                }
            }
        }
        rounds++;
        int all_nonzero = 1;
        for (i = RATE_LANES; i < TOTAL_LANES; i++) {
            if (lanes[i] == 0) {
                all_nonzero = 0;
                break;
            }
        }
        if (all_nonzero)
            break;
    }

    printf("All capacity lanes became non-zero after %d rounds (no permutation).\n", rounds);
    return 0;
}
