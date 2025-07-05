#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 8

void xor_encrypt_decrypt(uint8_t *block, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key[i];
    }
}

void pad(uint8_t *data, int original_len, int total_len) {
    data[original_len] = 0x80;
    for (int i = original_len + 1; i < total_len; i++)
        data[i] = 0x00;
}

void ecb_mode(uint8_t *plaintext, uint8_t *key, int blocks) {
    printf("\nECB Mode:\n");
    for (int i = 0; i < blocks; i++) {
        xor_encrypt_decrypt(&plaintext[i * BLOCK_SIZE], key);
        printf("Block %d: ", i);
        for (int j = 0; j < BLOCK_SIZE; j++)
            printf("%02X ", plaintext[i * BLOCK_SIZE + j]);
        printf("\n");
    }
}

void cbc_mode(uint8_t *plaintext, uint8_t *key, uint8_t *iv, int blocks) {
    printf("\nCBC Mode:\n");
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++)
            plaintext[i * BLOCK_SIZE + j] ^= iv[j];

        xor_encrypt_decrypt(&plaintext[i * BLOCK_SIZE], key);

        memcpy(iv, &plaintext[i * BLOCK_SIZE], BLOCK_SIZE);
        printf("Block %d: ", i);
        for (int j = 0; j < BLOCK_SIZE; j++)
            printf("%02X ", plaintext[i * BLOCK_SIZE + j]);
        printf("\n");
    }
}

void cfb_mode(uint8_t *plaintext, uint8_t *key, uint8_t *iv, int blocks) {
    printf("\nCFB Mode:\n");
    for (int i = 0; i < blocks; i++) {
        uint8_t temp[BLOCK_SIZE];
        memcpy(temp, iv, BLOCK_SIZE);
        xor_encrypt_decrypt(temp, key);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i * BLOCK_SIZE + j] ^= temp[j];
            iv[j] = plaintext[i * BLOCK_SIZE + j];
        }

        printf("Block %d: ", i);
        for (int j = 0; j < BLOCK_SIZE; j++)
            printf("%02X ", plaintext[i * BLOCK_SIZE + j]);
        printf("\n");
    }
}

int main() {
    uint8_t key[BLOCK_SIZE] = {0x10, 0x22, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
    uint8_t iv[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0xAA, 0xBB, 0xCC, 0xDD};

    char message[] = "HELLO123";
    int original_len = strlen(message);

    int total_len = ((original_len / BLOCK_SIZE) + 1) * BLOCK_SIZE;
    uint8_t plaintext[64] = {0};
    memcpy(plaintext, message, original_len);
    pad(plaintext, original_len, total_len);

    uint8_t ecb_text[64];
    memcpy(ecb_text, plaintext, total_len);
    ecb_mode(ecb_text, key, total_len / BLOCK_SIZE);

    uint8_t cbc_text[64];
    memcpy(cbc_text, plaintext, total_len);
    uint8_t cbc_iv[BLOCK_SIZE];
    memcpy(cbc_iv, iv, BLOCK_SIZE);
    cbc_mode(cbc_text, key, cbc_iv, total_len / BLOCK_SIZE);

    uint8_t cfb_text[64];
    memcpy(cfb_text, plaintext, total_len);
    uint8_t cfb_iv[BLOCK_SIZE];
    memcpy(cfb_iv, iv, BLOCK_SIZE);
    cfb_mode(cfb_text, key, cfb_iv, total_len / BLOCK_SIZE);

    return 0;
}
