#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 16  // 128 bits = 16 bytes

// XOR a block with Rb
void xor_rb(unsigned char *block, unsigned char rb) {
    block[BLOCK_SIZE - 1] ^= rb;
}

// Left shift 128-bit block by 1 bit
void left_shift(unsigned char *input, unsigned char *output) {
    unsigned char overflow = 0;
    for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
        output[i] = (input[i] << 1) | overflow;
        overflow = (input[i] & 0x80) ? 1 : 0;
    }
}

// Check MSB of a block
int msb_is_one(unsigned char *block) {
    return (block[0] & 0x80) != 0;
}

// Print a block in hex
void print_block(const char *label, unsigned char *block) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++)
        printf("%02X ", block[i]);
    printf("\n");
}

int main() {
    unsigned char L[BLOCK_SIZE] = {
        0x6C, 0x3E, 0xA0, 0x17, 0x65, 0x28, 0x4F, 0xE2,
        0x02, 0x84, 0xE8, 0xBC, 0x9B, 0x1A, 0x0B, 0x73
    };  // Example E_K(0^128), should be from AES-128 encryption

    unsigned char K1[BLOCK_SIZE] = {0};
    unsigned char K2[BLOCK_SIZE] = {0};

    // Step 1: Generate K1
    left_shift(L, K1);
    if (msb_is_one(L)) {
        xor_rb(K1, 0x87);  // Rb for 128-bit block
    }

    // Step 2: Generate K2
    left_shift(K1, K2);
    if (msb_is_one(K1)) {
        xor_rb(K2, 0x87);  // Rb again
    }

    // Print results
    print_block("L  (E_K(0^128))", L);
    print_block("K1", K1);
    print_block("K2", K2);

    return 0;
}
