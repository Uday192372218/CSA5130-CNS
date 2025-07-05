#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 8

// XOR "encryption" block (toy cipher for demo)
void encrypt_block(unsigned char *in, unsigned char *key, unsigned char *out) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        out[i] = in[i] ^ key[i];
}

// Padding: always add a padding block, even if message is already aligned
int pad_message(unsigned char *input, int len, unsigned char *padded) {
    int pad_len = BLOCK_SIZE - (len % BLOCK_SIZE);
    if (pad_len == 0) pad_len = BLOCK_SIZE;

    memcpy(padded, input, len);
    padded[len] = 0x80;
    for (int i = len + 1; i < len + pad_len; i++)
        padded[i] = 0x00;

    return len + pad_len;
}

// Print hex
void print_hex(const char *label, unsigned char *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

// ECB Mode
void ECB_encrypt(unsigned char *plaintext, int len, unsigned char *key, unsigned char *ciphertext) {
    for (int i = 0; i < len; i += BLOCK_SIZE)
        encrypt_block(plaintext + i, key, ciphertext + i);
}

// CBC Mode
void CBC_encrypt(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char block[BLOCK_SIZE];
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE; j++)
            block[j] = plaintext[i + j] ^ iv[j];
        encrypt_block(block, key, ciphertext + i);
        memcpy(iv, ciphertext + i, BLOCK_SIZE);  // update IV
    }
}

// CFB Mode (simplified: 8-bit feedback)
void CFB_encrypt(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char cipher_iv[BLOCK_SIZE];
    for (int i = 0; i < len; i++) {
        encrypt_block(iv, key, cipher_iv);
        ciphertext[i] = plaintext[i] ^ cipher_iv[0];

        // shift IV and add new byte
        memmove(iv, iv + 1, BLOCK_SIZE - 1);
        iv[BLOCK_SIZE - 1] = ciphertext[i];
    }
}

int main() {
    unsigned char key[BLOCK_SIZE] = "KEY@123";
    unsigned char iv_cbc[BLOCK_SIZE] = {0};
    unsigned char iv_cfb[BLOCK_SIZE] = {0};

    unsigned char plaintext[] = "HELLO123";  // 8 bytes
    unsigned char padded[64] = {0};
    int padded_len = pad_message(plaintext, strlen((char*)plaintext), padded);

    unsigned char ciphertext_ecb[64] = {0};
    unsigned char ciphertext_cbc[64] = {0};
    unsigned char ciphertext_cfb[64] = {0};

    // ECB
    ECB_encrypt(padded, padded_len, key, ciphertext_ecb);
    print_hex("ECB", ciphertext_ecb, padded_len);

    // CBC
    CBC_encrypt(padded, padded_len, key, iv_cbc, ciphertext_cbc);
    print_hex("CBC", ciphertext_cbc, padded_len);

    // CFB
    CFB_encrypt(padded, padded_len, key, iv_cfb, ciphertext_cfb);
    print_hex("CFB", ciphertext_cfb, padded_len);

    return 0;
}
