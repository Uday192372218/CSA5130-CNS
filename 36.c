#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Calculate modular inverse of a under modulo m
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1; // No inverse exists
}

// Encryption function: C = (aP + b) % 26
char affine_encrypt_char(char p, int a, int b) {
    if (p >= 'A' && p <= 'Z') {
        return ((a * (p - 'A') + b) % 26) + 'A';
    } else if (p >= 'a' && p <= 'z') {
        return ((a * (p - 'a') + b) % 26) + 'a';
    }
    return p;
}

// Decryption function: P = a_inv * (C - b) % 26
char affine_decrypt_char(char c, int a, int b) {
    int a_inv = mod_inverse(a, 26);
    if (a_inv == -1) {
        printf("Invalid key 'a': no modular inverse exists.\n");
        exit(1);
    }

    if (c >= 'A' && c <= 'Z') {
        return ((a_inv * ((c - 'A') - b + 26)) % 26) + 'A';
    } else if (c >= 'a' && c <= 'z') {
        return ((a_inv * ((c - 'a') - b + 26)) % 26) + 'a';
    }
    return c;
}

void affine_encrypt(char *plaintext, char *ciphertext, int a, int b) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        ciphertext[i] = affine_encrypt_char(plaintext[i], a, b);
    }
    ciphertext[len] = '\0';
}

void affine_decrypt(char *ciphertext, char *plaintext, int a, int b) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        plaintext[i] = affine_decrypt_char(ciphertext[i], a, b);
    }
    plaintext[len] = '\0';
}

int main() {
    char text[100], encrypted[100], decrypted[100];
    int a, b;

    printf("Enter plaintext (A-Z or a-z): ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;  // Remove newline

    printf("Enter key 'a' (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter key 'b': ");
    scanf("%d", &b);

    if (mod_inverse(a, 26) == -1) {
        printf("Error: 'a' = %d is not valid (no modular inverse mod 26).\n", a);
        return 1;
    }

    affine_encrypt(text, encrypted, a, b);
    printf("Encrypted text: %s\n", encrypted);

    affine_decrypt(encrypted, decrypted, a, b);
    printf("Decrypted text: %s\n", decrypted);

    return 0;
}
