#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8

void xor_encrypt_block(unsigned char *block, unsigned char key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key;
    }
}

void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

void simulate_ecb(unsigned char *plaintext, int len, unsigned char key) {
    unsigned char ciphertext[64];
    unsigned char decrypted[64];
    int blocks = len / BLOCK_SIZE;

    for (int i = 0; i < blocks; i++) {
        memcpy(&ciphertext[i * BLOCK_SIZE], &plaintext[i * BLOCK_SIZE], BLOCK_SIZE);
        xor_encrypt_block(&ciphertext[i * BLOCK_SIZE], key);
    }

    ciphertext[8] ^= 0xFF;

    for (int i = 0; i < blocks; i++) {
        memcpy(&decrypted[i * BLOCK_SIZE], &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
        xor_encrypt_block(&decrypted[i * BLOCK_SIZE], key);
    }
    decrypted[len] = '\0';

    printf("\nECB Mode:\n");
    printf("Ciphertext (with error): ");
    print_hex(ciphertext, len);
    printf("Decrypted: %s\n", decrypted);
}

void simulate_cbc(unsigned char *plaintext, int len, unsigned char key, unsigned char iv) {
    unsigned char ciphertext[64];
    unsigned char decrypted[64];
    unsigned char prev_block[BLOCK_SIZE];
    int blocks = len / BLOCK_SIZE;

    memcpy(prev_block, &iv, BLOCK_SIZE);
    for (int i = 0; i < blocks; i++) {
        unsigned char temp[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++)
            temp[j] = plaintext[i * BLOCK_SIZE + j] ^ prev_block[j];
        xor_encrypt_block(temp, key);
        memcpy(&ciphertext[i * BLOCK_SIZE], temp, BLOCK_SIZE);
        memcpy(prev_block, temp, BLOCK_SIZE);
    }

    ciphertext[4] ^= 0xFF;

    memcpy(prev_block, &iv, BLOCK_SIZE);
    for (int i = 0; i < blocks; i++) {
        unsigned char temp[BLOCK_SIZE];
        memcpy(temp, &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
        xor_encrypt_block(temp, key);
        for (int j = 0; j < BLOCK_SIZE; j++)
            decrypted[i * BLOCK_SIZE + j] = temp[j] ^ prev_block[j];
        memcpy(prev_block, &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
    }
    decrypted[len] = '\0';

    printf("\nCBC Mode:\n");
    printf("Ciphertext (with error in C1): ");
    print_hex(ciphertext, len);
    printf("Decrypted: %s\n", decrypted);
}

int main() {
    unsigned char plaintext[] = "Block1__Block2__Block3__";
    unsigned char key = 0x3C;
    unsigned char iv[BLOCK_SIZE] = {0x00};

    printf("Original: %s\n", plaintext);

    simulate_ecb(plaintext, strlen((char *)plaintext), key);
    simulate_cbc(plaintext, strlen((char *)plaintext), key, iv[0]);

    return 0;
}
