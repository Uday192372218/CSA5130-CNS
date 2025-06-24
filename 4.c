#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to generate the full key based on plaintext length
void generateFullKey(char *plaintext, char *key, char *fullKey) {
    int i, j, keyLen = strlen(key), textLen = strlen(plaintext);
    for (i = 0, j = 0; i < textLen; i++) {
        if (isalpha(plaintext[i])) {
            fullKey[i] = toupper(key[j % keyLen]);
            j++;
        } else {
            fullKey[i] = plaintext[i];  // keep non-letters as is
        }
    }
    fullKey[textLen] = '\0';
}

// Function to encrypt plaintext using Vigenère cipher
void encrypt(char *plaintext, char *key, char *ciphertext) {
    char fullKey[1000];
    generateFullKey(plaintext, key, fullKey);
    int i;

    for (i = 0; i < strlen(plaintext); i++) {
        char p = plaintext[i];
        char k = fullKey[i];
        if (isupper(p)) {
            ciphertext[i] = ((p - 'A' + (k - 'A')) % 26) + 'A';
        }
        else if (islower(p)) {
            ciphertext[i] = ((p - 'a' + (k - 'A')) % 26) + 'a';
        }
        else {
            ciphertext[i] = p; // non-alphabetic characters are unchanged
        }
    }
    ciphertext[i] = '\0';
}

int main() {
    char plaintext[1000], key[100], ciphertext[1000];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0;

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    // convert key to uppercase
    for (int i = 0; i < strlen(key); i++) {
        key[i] = toupper(key[i]);
    }

    encrypt(plaintext, key, ciphertext);

    printf("Encrypted text: %s\n", ciphertext);

    return 0;
}
