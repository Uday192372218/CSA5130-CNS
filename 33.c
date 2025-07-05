#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 64
#define HALF_BLOCK 32
#define NUM_ROUNDS 16

// Initial Permutation Table
int IP[64] = {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};

// Final Permutation Table
int FP[64] = {
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25
};

// Simple Expansion (E) Table
int E[48] = {
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1
};

// Permutation Function P
int P[32] = {
    16,7,20,21,
    29,12,28,17,
    1,15,23,26,
    5,18,31,10,
    2,8,24,14,
    32,27,3,9,
    19,13,30,6,
    22,11,4,25
};

// S-box (only S-box 1 for simplicity)
int SBOX[4][16] = {
    {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
    {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
    {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
    {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
};

// Simple function to perform permutation
void permute(int *table, int n, unsigned char *in, unsigned char *out) {
    for (int i = 0; i < n; i++) {
        int bit = (in[(table[i] - 1) / 8] >> (7 - ((table[i] - 1) % 8))) & 1;
        out[i / 8] |= (bit << (7 - (i % 8)));
    }
}

// Expand 32-bit block to 48 bits
void expand(unsigned char *in, unsigned char *out) {
    for (int i = 0; i < 48; i++) {
        int bit = (in[(E[i] - 1) / 8] >> (7 - ((E[i] - 1) % 8))) & 1;
        out[i / 8] |= (bit << (7 - (i % 8)));
    }
}

// S-box substitution (only 1 S-box for demo)
unsigned char sbox_sub(unsigned char in6) {
    int row = ((in6 & 0x20) >> 4) | (in6 & 0x01);
    int col = (in6 >> 1) & 0x0F;
    return SBOX[row][col];
}

// XOR n bytes
void xor_bytes(unsigned char *a, unsigned char *b, unsigned char *out, int len) {
    for (int i = 0; i < len; i++)
        out[i] = a[i] ^ b[i];
}

// DES round (simplified)
void des_round(unsigned char *left, unsigned char *right, unsigned char *subkey) {
    unsigned char expanded[6] = {0};
    expand(right, expanded);

    unsigned char xored[6] = {0};
    xor_bytes(expanded, subkey, xored, 6);

    // Apply 1 S-box only (real DES has 8)
    unsigned char sbox_out[4] = {0};
    sbox_out[0] = sbox_sub(xored[0] >> 2);  // using first 6 bits

    // Permute (P)
    unsigned char p_out[4] = {0};
    permute(P, 32, sbox_out, p_out);

    unsigned char new_right[4] = {0};
    xor_bytes(left, p_out, new_right, 4);

    // Copy old right to left
    memcpy(left, right, 4);
    memcpy(right, new_right, 4);
}

// Encrypt block
void des_encrypt_block(unsigned char *plaintext, unsigned char *ciphertext, unsigned char *key) {
    unsigned char ip[8] = {0};
    permute(IP, 64, plaintext, ip);

    unsigned char left[4], right[4];
    memcpy(left, ip, 4);
    memcpy(right, ip + 4, 4);

    // Use same subkey for all rounds for simplicity
    for (int i = 0; i < NUM_ROUNDS; i++) {
        des_round(left, right, key);
    }

    // Combine halves
    unsigned char combined[8];
    memcpy(combined, right, 4);
    memcpy(combined + 4, left, 4);  // final swap

    permute(FP, 64, combined, ciphertext);
}

int main() {
    unsigned char plaintext[8] = {0x12, 0x34, 0xAB, 0xCD, 0x56, 0x78, 0x90, 0xEF};
    unsigned char key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    unsigned char ciphertext[8] = {0};

    des_encrypt_block(plaintext, ciphertext, key);

    printf("Ciphertext: ");
    for (int i = 0; i < 8; i++)
        printf("%02X ", ciphertext[i]);
    printf("\n");

    return 0;
}
