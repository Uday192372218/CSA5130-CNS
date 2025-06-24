#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to calculate GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

// Function to find modular inverse
int modInverse(int a) {
    a = a % 26;
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1)
            return x;
    }
    return -1;
}

// Encryption function
void encrypt(char plaintext[], int a, int b, char ciphertext[]) {
    int i;
    for (i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (isupper(ch)) {
            ciphertext[i] = ((a * (ch - 'A') + b) % 26) + 'A';
        } else if (islower(ch)) {
            ciphertext[i] = ((a * (ch - 'a') + b) % 26) + 'a';
        } else {
            ciphertext[i] = ch;
        }
    }
    ciphertext[i] = '\0';
}

// Decryption function
void decrypt(char ciphertext[], int a, int b, char plaintext[]) {
    int a_inv = modInverse(a);
    if (a_inv == -1) {
        printf("Invalid 'a', modular inverse doesn't exist.\n");
        return;
    }
    int i;
    for (i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isupper(ch)) {
            plaintext[i] = (a_inv * ((ch - 'A' - b + 26) % 26)) % 26 + 'A';
        } else if (islower(ch)) {
            plaintext[i] = (a_inv * ((ch - 'a' - b + 26) % 26)) % 26 + 'a';
        } else {
            plaintext[i] = ch;
        }
    }
    plaintext[i] = '\0';
}

int main() {
    char plaintext[1000], ciphertext[1000], decrypted[1000];
    int a, b;

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0;

    printf("Enter key 'a' (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter key 'b' (0 to 25): ");
    scanf("%d", &b);

    if (gcd(a, 26) != 1) {
        printf("Invalid value of 'a'. It must be coprime with 26.\n");
        return 1;
    }

    encrypt(plaintext, a, b, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);

    decrypt(ciphertext, a, b, decrypted);
    printf("Decrypted text: %s\n", decrypted);

    return 0;
}
