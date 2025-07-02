#include <stdio.h>
#include <stdint.h>
#include <string.h>
int shift_schedule[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};
int PC1_C[28] = {
     57,49,41,33,25,17,9,
      1,58,50,42,34,26,18,
     10, 2,59,51,43,35,27,
     19,11, 3,60,52,44,36
};

int PC1_D[28] = {
     63,55,47,39,31,23,15,
      7,62,54,46,38,30,22,
     14, 6,61,53,45,37,29,
     21,13, 5,28,20,12,4
};
int get_bit(uint64_t key, int pos) {
    return (key >> (64 - pos)) & 0x01;
}
void set_bit(uint64_t *key, int pos, int value) {
    if (value)
        *key |= ((uint64_t)1 << (48 - pos));
}
uint32_t left_rotate28(uint32_t val, int shifts) {
    return ((val << shifts) | (val >> (28 - shifts))) & 0x0FFFFFFF;
}
void generate_subkeys(uint64_t key64, uint64_t subkeys[16]) {
    uint32_t C = 0, D = 0;
    for (int i = 0; i < 28; i++) {
        C <<= 1;
        C |= get_bit(key64, PC1_C[i]);

        D <<= 1;
        D |= get_bit(key64, PC1_D[i]);
    }
    for (int round = 0; round < 16; round++) {
        C = left_rotate28(C, shift_schedule[round]);
        D = left_rotate28(D, shift_schedule[round]);
        uint64_t subkey = 0;
        for (int i = 27; i >= 4; i--) {
            int bit = (C >> i) & 1;
            set_bit(&subkey, 28 - i, bit);
        }
        for (int i = 27; i >= 4; i--) {
            int bit = (D >> i) & 1;
            set_bit(&subkey, 52 - i, bit);
        }
        subkeys[round] = subkey;
    }
}
void print_subkeys(uint64_t subkeys[16]) {
    for (int i = 0; i < 16; i++) {
        printf("Subkey %2d: ", i + 1);
        for (int j = 47; j >= 0; j--) {
            printf("%d", (int)((subkeys[i] >> j) & 1));
        }
        printf("\n");
    }
}
int main() {
    uint64_t key = 0x133457799BBCDFF1;  // Example 64-bit key
    uint64_t subkeys[16];
    generate_subkeys(key, subkeys);
    print_subkeys(subkeys);
    return 0;
}
