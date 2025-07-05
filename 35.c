#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100

// Function to encrypt using one-time pad Vigenère
void encrypt(const char *plaintext, int *key, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = ((plaintext[i] - 'A' + key[i]) % 26) + 'A';
        } else {
            ciphertext[i] = plaintext[i]; // non-alphabetic chars unchanged
        }
    }
    ciphertext[len] = '\0';
}

// Function to decrypt
void decrypt(const char *ciphertext, int *key, char *decrypted) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            decrypted[i] = ((ciphertext[i] - 'A' - key[i] + 26) % 26) + 'A';
        } else {
            decrypted[i] = ciphertext[i];
        }
    }
    decrypted[len] = '\0';
}

// Generate random key of same length as plaintext
void generate_key(int *key, int len) {
    for (int i = 0; i < len; i++) {
        key[i] = rand() % 26;  // Random shift: 0–25
    }
}

int main() {
    char plaintext[MAX_LEN];
    char ciphertext[MAX_LEN];
    char decrypted[MAX_LEN];
    int key[MAX_LEN];

    srand(time(NULL));

    printf("Enter UPPERCASE plaintext (A–Z only): ");
    fgets(plaintext, MAX_LEN, stdin);
    plaintext[strcspn(plaintext, "\n")] = 0; // remove newline

    int len = strlen(plaintext);
    generate_key(key, len);

    encrypt(plaintext, key, ciphertext);
    decrypt(ciphertext, key, decrypted);

    printf("\nGenerated Key (shifts): ");
    for (int i = 0; i < len; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
            printf("%d ", key[i]);
        else
            printf("x ");
    }

    printf("\nCiphertext: %s\n", ciphertext);
    printf("Decrypted : %s\n", decrypted);

    return 0;
}
