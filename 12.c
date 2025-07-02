#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 2

// Key matrix for encryption
int key[SIZE][SIZE] = {
    {9, 4},
    {5, 7}
};

// Function to calculate modular inverse of determinant
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

// Function to prepare text (remove non-letters, uppercase, pad with X)
void prepareText(const char* input, char* output) {
    int i = 0, j = 0;
    while (input[i]) {
        if (isalpha(input[i])) {
            output[j++] = toupper(input[i]);
        }
        i++;
    }
    if (j % 2 != 0) output[j++] = 'X'; // Padding
    output[j] = '\0';
}

// Encrypt one pair using Hill cipher
void encryptPair(char a, char b, char* out) {
    int p1 = a - 'A';
    int p2 = b - 'A';
    int c1 = (key[0][0] * p1 + key[0][1] * p2) % 26;
    int c2 = (key[1][0] * p1 + key[1][1] * p2) % 26;
    out[0] = c1 + 'A';
    out[1] = c2 + 'A';
}

// Decrypt one pair using inverse key matrix
void decryptPair(char a, char b, char* out, int invKey[2][2]) {
    int c1 = a - 'A';
    int c2 = b - 'A';
    int p1 = (invKey[0][0] * c1 + invKey[0][1] * c2) % 26;
    int p2 = (invKey[1][0] * c1 + invKey[1][1] * c2) % 26;
    if (p1 < 0) p1 += 26;
    if (p2 < 0) p2 += 26;
    out[0] = p1 + 'A';
    out[1] = p2 + 'A';
}

// Function to compute inverse key matrix mod 26
int getInverseMatrix(int invKey[2][2]) {
    int det = key[0][0]*key[1][1] - key[0][1]*key[1][0];
    det = (det % 26 + 26) % 26;
    int detInv = modInverse(det, 26);
    if (detInv == -1) return 0;

    // Adjoint matrix and multiply by det inverse mod 26
    invKey[0][0] =  key[1][1] * detInv % 26;
    invKey[0][1] = -key[0][1] * detInv % 26;
    invKey[1][0] = -key[1][0] * detInv % 26;
    invKey[1][1] =  key[0][0] * detInv % 26;

    // Ensure all values are positive mod 26
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            invKey[i][j] = (invKey[i][j] % 26 + 26) % 26;

    return 1;
}

int main() {
    const char* plaintext = "meet me at the usual place at ten rather than eight oclock";
    char prepared[200], encrypted[200], decrypted[200];
    int invKey[2][2];

    printf("Plaintext: %s\n", plaintext);

    prepareText(plaintext, prepared);
    printf("\nPrepared (cleaned) text: %s\n", prepared);

    // Encryption
    int len = strlen(prepared);
    for (int i = 0; i < len; i += 2) {
        encryptPair(prepared[i], prepared[i+1], &encrypted[i]);
    }
    encrypted[len] = '\0';
    printf("\nEncrypted text: %s\n", encrypted);

    // Inverse key matrix
    if (!getInverseMatrix(invKey)) {
        printf("Key matrix is not invertible modulo 26.\n");
        return 1;
    }

    // Decryption
    for (int i = 0; i < len; i += 2) {
        decryptPair(encrypted[i], encrypted[i+1], &decrypted[i], invKey);
    }
    decrypted[len] = '\0';
    printf("\nDecrypted text: %s\n", decrypted);

    return 0;
}
