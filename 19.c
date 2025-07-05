#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8

void simple_block_encrypt(unsigned char *block, unsigned char key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key; // simple XOR encryption (for demo only)
    }
}

void simple_block_decrypt(unsigned char *block, unsigned char key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key; // XOR again to decrypt
    }
}

int main() {
    unsigned char plaintext[] = "ThisIsECBMode12345678";  // must be multiple of BLOCK_SIZE
    unsigned char ciphertext[64];
    unsigned char decrypted[64];

    unsigned char key = 0x5A; // dummy key
    int len = strlen((char *)plaintext);
    int blocks = len / BLOCK_SIZE;

    printf("Original: %s\n", plaintext);

    // Encrypt block-by-block (ECB)
    for (int i = 0; i < blocks; i++) {
        memcpy(&ciphertext[i * BLOCK_SIZE], &plaintext[i * BLOCK_SIZE], BLOCK_SIZE);
        simple_block_encrypt(&ciphertext[i * BLOCK_SIZE], key);
    }

    printf("Ciphertext (hex): ");
    for (int i = 0; i < len; i++)
        printf("%02X ", ciphertext[i]);
    printf("\n");

    // Decrypt block-by-block (ECB)
    for (int i = 0; i < blocks; i++) {
        memcpy(&decrypted[i * BLOCK_SIZE], &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
        simple_block_decrypt(&decrypted[i * BLOCK_SIZE], key);
    }

    decrypted[len] = '\0';
    printf("Decrypted: %s\n", decrypted);

    return 0;
}
