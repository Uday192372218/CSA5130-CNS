#include <stdio.h>
#include <string.h>
#include <ctype.h>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

void decryptAffine(char ciphertext[], int a, int b) {
    int a_inv = modInverse(a, 26);
    if (a_inv == -1) {
        printf("No modular inverse for a=%d\n", a);
        return;
    }

    printf("Trying a = %d, b = %d, a_inv = %d\n", a, b, a_inv);
    printf("Decrypted text:\n");

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char c = ciphertext[i];
        if (isalpha(c)) {
            int C = toupper(c) - 'A';
            int P = (a_inv * (C - b + 26)) % 26;
            printf("%c", P + 'A');
        } else {
            printf("%c", c);
        }
    }
    printf("\n");
}
int main() {
    char ciphertext[] = "BUBUBUBUBU BUBUBU";
    int possible_a[] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    for (int i = 0; i < sizeof(possible_a)/sizeof(possible_a[0]); i++) {
        int a = possible_a[i];
        if ((15 * a) % 26 == 19) {
            int b = (1 - (a * 4) % 26 + 26) % 26;
            decryptAffine(ciphertext, a, b);
        }
    }

    return 0;
}
