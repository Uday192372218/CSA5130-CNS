#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8  // 8 bytes block size

// Simple XOR "encryption" for demonstration (not secure!)
void encrypt_block(unsigned char *input, unsigned char *key, unsigned char *output) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        output[i] = input[i] ^ key[i];
}

// CBC-MAC for any number of blocks
void cbc_mac(unsigned char *message, int blocks, unsigned char *key, unsigned char *mac) {
    unsigned char prev[BLOCK_SIZE] = {0};
    unsigned char temp[BLOCK_SIZE];

    for (int b = 0; b < blocks; b++) {
        for (int i = 0; i < BLOCK_SIZE; i++)
            temp[i] = message[b * BLOCK_SIZE + i] ^ prev[i];

        encrypt_block(temp, key, mac);  // E_K
        memcpy(prev, mac, BLOCK_SIZE);
    }
}

void print_block(const char *label, unsigned char *block) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++)
        printf("%02X ", block[i]);
    printf("\n");
}

int main() {
    unsigned char key[BLOCK_SIZE] = {0x0A, 0x1B, 0x2C, 0x3D, 0x4E, 0x5F, 0x6A, 0x7B};

    unsigned char X[BLOCK_SIZE] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
    unsigned char T[BLOCK_SIZE];

    // Step 1: MAC of one-block message X
    cbc_mac(X, 1, key, T);
    print_block("MAC(K, X) = T", T);

    // Step 2: Construct second block as (X ⊕ T)
    unsigned char second_block[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
        second_block[i] = X[i] ^ T[i];

    // Step 3: Concatenate X || (X ⊕ T)
    unsigned char forged_msg[BLOCK_SIZE * 2];
    memcpy(forged_msg, X, BLOCK_SIZE);
    memcpy(forged_msg + BLOCK_SIZE, second_block, BLOCK_SIZE);

    unsigned char forged_mac[BLOCK_SIZE];
    cbc_mac(forged_msg, 2, key, forged_mac);

    print_block("MAC(K, X || (X⊕T))", forged_mac);

    // Verify MAC matches T
    if (memcmp(forged_mac, T, BLOCK_SIZE) == 0)
        printf("\n✅ Forgery successful! MAC(X || (X⊕T)) = MAC(X)\n");
    else
        printf("\n❌ Forgery failed.\n");

    return 0;
}
