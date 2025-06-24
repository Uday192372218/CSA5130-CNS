#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char *plaintext, char *ciphertext_alphabet) {
    int i;
    char ch;

    for (i = 0; plaintext[i] != '\0'; i++) {
        ch = plaintext[i];

        if (isupper(ch)) {
            plaintext[i] = ciphertext_alphabet[ch - 'A'];
        }
        else if (islower(ch)) {
            plaintext[i] = tolower(ciphertext_alphabet[ch - 'a']);
        }
    }
}

int main() {
    char plaintext[1000];
    // Example substitution alphabet (must be 26 characters, all unique)
    char ciphertext_alphabet[27] = "QWERTYUIOPASDFGHJKLZXCVBNM";

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    encrypt(plaintext, ciphertext_alphabet);

    printf("Encrypted text: %s\n", plaintext);

    return 0;
}
