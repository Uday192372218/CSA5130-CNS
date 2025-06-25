#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26

// Function to create cipher alphabet using a keyword
void create_cipher_alphabet(char *keyword, char cipher[]) {
    int used[26] = {0};
    int i, j = 0;

    // Convert keyword to uppercase
    for (i = 0; keyword[i]; i++) {
        keyword[i] = toupper(keyword[i]);
    }

    // Add unique keyword letters first
    for (i = 0; keyword[i]; i++) {
        if (!used[keyword[i] - 'A']) {
            cipher[j++] = keyword[i];
            used[keyword[i] - 'A'] = 1;
        }
    }

    // Add remaining unused letters
    for (i = 0; i < 26; i++) {
        if (!used[i]) {
            cipher[j++] = 'A' + i;
        }
    }
    cipher[ALPHABET_LEN] = '\0'; // Null-terminate the cipher string
}

// Encrypt a plaintext using the cipher alphabet
void encrypt(char *plaintext, char *cipher, char *ciphertext) {
    int i;
    for (i = 0; plaintext[i]; i++) {
        char ch = tolower(plaintext[i]);
        if (ch >= 'a' && ch <= 'z') {
            ciphertext[i] = tolower(cipher[ch - 'a']);
        } else {
            ciphertext[i] = plaintext[i]; // non-letter characters unchanged
        }
    }
    ciphertext[i] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipher[27];
    char plaintext[1000], ciphertext[1000];

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    create_cipher_alphabet(keyword, cipher);

    printf("\nGenerated Cipher Alphabet:\n");
    for (int i = 0; i < 26; i++) {
        printf("%c -> %c\n", 'a' + i, tolower(cipher[i]));
    }

    encrypt(plaintext, cipher, ciphertext);

    printf("\nEncrypted text: %s\n", ciphertext);

    return 0;
}
