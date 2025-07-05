#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;

BYTE IP[8]  = {1, 5, 2, 0, 3, 7, 4, 6};
BYTE IP_INV[8] = {3, 0, 2, 4, 6, 1, 7, 5};
BYTE EP[8]  = {3, 0, 1, 2, 1, 2, 3, 0};
BYTE P4[4]  = {1, 3, 2, 0};
BYTE P10[10]= {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
BYTE P8[8]  = {5, 2, 6, 3, 7, 4, 9, 8};

BYTE S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

BYTE S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Utility functions
BYTE permute(BYTE in, BYTE* p, int len) {
    BYTE out = 0;
    for (int i = 0; i < len; i++) {
        out |= ((in >> (7 - p[i])) & 1) << (len - 1 - i);
    }
    return out;
}

BYTE left_shift_5bit(BYTE k, int n) {
    return ((k << n) | (k >> (5 - n))) & 0x1F;
}

WORD key_gen(WORD key, BYTE* k1, BYTE* k2) {
    WORD p10 = 0;
    for (int i = 0; i < 10; i++)
        p10 |= ((key >> (9 - P10[i])) & 1) << (9 - i);

    BYTE left = (p10 >> 5) & 0x1F;
    BYTE right = p10 & 0x1F;

    left = left_shift_5bit(left, 1);
    right = left_shift_5bit(right, 1);
    WORD joined = (left << 5) | right;

    *k1 = 0;
    for (int i = 0; i < 8; i++)
        *k1 |= ((joined >> (9 - P8[i])) & 1) << (7 - i);

    left = left_shift_5bit(left, 2);
    right = left_shift_5bit(right, 2);
    joined = (left << 5) | right;

    *k2 = 0;
    for (int i = 0; i < 8; i++)
        *k2 |= ((joined >> (9 - P8[i])) & 1) << (7 - i);

    return 0;
}

BYTE sbox_output(BYTE in, BYTE sbox[4][4]) {
    BYTE row = ((in & 0x8) >> 2) | (in & 0x1);
    BYTE col = (in >> 1) & 0x3;
    return sbox[row][col];
}

BYTE fk(BYTE input, BYTE subkey) {
    BYTE left = input >> 4;
    BYTE right = input & 0xF;

    BYTE ep = 0;
    for (int i = 0; i < 8; i++)
        ep |= ((right >> (3 - EP[i])) & 1) << (7 - i);

    BYTE temp = ep ^ subkey;
    BYTE s0_out = sbox_output(temp >> 4, S0);
    BYTE s1_out = sbox_output(temp & 0x0F, S1);

    BYTE s_out = (s0_out << 2) | s1_out;
    BYTE p4 = 0;
    for (int i = 0; i < 4; i++)
        p4 |= ((s_out >> (3 - P4[i])) & 1) << (3 - i);

    return ((left ^ p4) << 4) | right;
}

BYTE sdes_encrypt(BYTE plaintext, BYTE k1, BYTE k2) {
    BYTE ip = permute(plaintext, IP, 8);
    BYTE fk1 = fk(ip, k1);
    BYTE swapped = (fk1 << 4) | (fk1 >> 4);
    BYTE fk2 = fk(swapped, k2);
    return permute(fk2, IP_INV, 8);
}

BYTE sdes_decrypt(BYTE ciphertext, BYTE k1, BYTE k2) {
    BYTE ip = permute(ciphertext, IP, 8);
    BYTE fk1 = fk(ip, k2);
    BYTE swapped = (fk1 << 4) | (fk1 >> 4);
    BYTE fk2 = fk(swapped, k1);
    return permute(fk2, IP_INV, 8);
}

// CBC wrappers
void encrypt_cbc(BYTE* pt, BYTE* ct, int len, BYTE k1, BYTE k2, BYTE iv) {
    BYTE prev = iv;
    for (int i = 0; i < len; i++) {
        BYTE block = pt[i] ^ prev;
        ct[i] = sdes_encrypt(block, k1, k2);
        prev = ct[i];
    }
}

void decrypt_cbc(BYTE* ct, BYTE* pt, int len, BYTE k1, BYTE k2, BYTE iv) {
    BYTE prev = iv;
    for (int i = 0; i < len; i++) {
        BYTE block = sdes_decrypt(ct[i], k1, k2);
        pt[i] = block ^ prev;
        prev = ct[i];
    }
}

int main() {
    BYTE pt[2] = {0x01, 0x23};
    BYTE ct[2] = {0};
    BYTE decrypted[2] = {0};
    BYTE k1, k2;
    WORD key = 0x1FD;
    BYTE iv = 0xAA;

    key_gen(key, &k1, &k2);

    encrypt_cbc(pt, ct, 2, k1, k2, iv);

    printf("Ciphertext: ");
    for (int i = 0; i < 2; i++)
        printf("%02X ", ct[i]);

    decrypt_cbc(ct, decrypted, 2, k1, k2, iv);

    printf("\nDecrypted:  ");
    for (int i = 0; i < 2; i++)
        printf("%02X ", decrypted[i]);

    return 0;
}
