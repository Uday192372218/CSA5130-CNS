#include <stdio.h>
#include <stdint.h>

uint8_t SBOX1[4][4] = {{1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2}};
uint8_t SBOX2[4][4] = {{0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3}};

uint8_t P4(uint8_t input) {
    return ((input & 1) << 3) | ((input & 2) << 1) | ((input & 4) >> 1) | ((input & 8) >> 3);
}
uint8_t IP(uint8_t input) {
    return ((input & 0x80) >> 1) | ((input & 0x40) >> 3) | ((input & 0x20) << 1) | ((input & 0x10) >> 1) |
           ((input & 0x08) << 1) | ((input & 0x04) << 2) | ((input & 0x02) >> 1) | ((input & 0x01) << 2);
}
uint8_t IP_INV(uint8_t input) {
    return ((input & 0x40) << 1) | ((input & 0x10) << 2) | ((input & 0x04) << 1) | ((input & 0x01) << 3) |
           ((input & 0x80) >> 1) | ((input & 0x20) >> 2) | ((input & 0x08) >> 1) | ((input & 0x02) >> 2);
}
uint8_t F(uint8_t right, uint8_t subkey) {
    uint8_t expanded = ((right & 0x08) << 1) | ((right & 0x01) << 2) |
                       ((right & 0x02) << 2) | ((right & 0x04) << 1);
    uint8_t temp = expanded ^ subkey;
    uint8_t row1 = ((temp & 0x08) >> 2) | (temp & 0x01);
    uint8_t col1 = (temp & 0x06) >> 1;
    uint8_t s1 = SBOX1[row1][col1];
    uint8_t row2 = ((temp & 0x80) >> 6) | ((temp & 0x10) >> 4);
    uint8_t col2 = (temp & 0x60) >> 5;
    uint8_t s2 = SBOX2[row2][col2];
    return P4((s1 << 2) | s2);
}
uint8_t S_DES(uint8_t plaintext, uint8_t k1, uint8_t k2) {
    plaintext = IP(plaintext);
    uint8_t left = (plaintext & 0xF0) >> 4;
    uint8_t right = plaintext & 0x0F;
    uint8_t temp = left ^ F(right, k1);
    left = right;
    right = temp;
    uint8_t preoutput = (left << 4) | right;
    return IP_INV((preoutput & 0xF0) | (F(preoutput & 0x0F, k2) ^ (preoutput >> 4)));
}
void encrypt_CTR_mode(uint8_t *plaintext, uint8_t *ciphertext, uint8_t key1, uint8_t key2, uint8_t counter, int blocks) {
    for (int i = 0; i < blocks; i++) {
        uint8_t keystream = S_DES(counter, key1, key2);
        ciphertext[i] = plaintext[i] ^ keystream;
        counter++;
    }
}
void print_binary(uint8_t byte) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (byte >> i) & 1);
}
int main() {
    uint8_t plaintext[3] = {0x01, 0x02, 0x04};
    uint8_t ciphertext[3], decrypted[3];
    uint8_t key1 = 0b11001100;
    uint8_t key2 = 0b10101010;
    uint8_t counter = 0x00;

    encrypt_CTR_mode(plaintext, ciphertext, key1, key2, counter, 3);
    printf("Ciphertext: ");
    for (int i = 0; i < 3; i++) {
        print_binary(ciphertext[i]);
        printf(" ");
    }
    encrypt_CTR_mode(ciphertext, decrypted, key1, key2, counter, 3);
    printf("\nDecrypted: ");
    for (int i = 0; i < 3; i++) {
        print_binary(decrypted[i]);
        printf(" ");
    }
    return 0;
}
