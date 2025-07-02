#include <stdio.h>
#include <stdlib.h>

// Function to compute mod inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

// Compute determinant of 2x2 matrix
int determinant(int key[2][2]) {
    return (key[0][0]*key[1][1] - key[0][1]*key[1][0]) % 26;
}

// Inverse of 2x2 matrix modulo 26
int inverseMatrix(int key[2][2], int inv[2][2]) {
    int det = determinant(key);
    det = (det + 26) % 26;
    int invDet = modInverse(det, 26);
    if (invDet == -1) return 0;

    inv[0][0] =  key[1][1] * invDet % 26;
    inv[0][1] = -key[0][1] * invDet % 26;
    inv[1][0] = -key[1][0] * invDet % 26;
    inv[1][1] =  key[0][0] * invDet % 26;

    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            inv[i][j] = (inv[i][j] + 26) % 26;

    return 1;
}

// Encrypt a pair of characters
void encrypt(char* pt, char* ct, int key[2][2]) {
    int p[2];
    p[0] = pt[0] - 'A';
    p[1] = pt[1] - 'A';

    for (int i=0; i<2; i++) {
        ct[i] = (key[i][0]*p[0] + key[i][1]*p[1]) % 26 + 'A';
    }
    ct[2] = '\0';
}

// Decrypt a pair of characters
void decrypt(char* ct, char* pt, int key[2][2]) {
    int inv[2][2];
    if (!inverseMatrix(key, inv)) {
        printf("Key is not invertible!\n");
        return;
    }

    int c[2];
    c[0] = ct[0] - 'A';
    c[1] = ct[1] - 'A';

    for (int i=0; i<2; i++) {
        pt[i] = (inv[i][0]*c[0] + inv[i][1]*c[1]) % 26 + 'A';
    }
    pt[2] = '\0';
}

// Known Plaintext Attack
void knownPlaintextAttack(char* pt, char* ct, int key[2][2]) {
    int P[2][2], C[2][2];

    for (int i=0; i<2; i++) {
        P[0][i] = pt[i] - 'A';
        P[1][i] = pt[i+2] - 'A';
        C[0][i] = ct[i] - 'A';
        C[1][i] = ct[i+2] - 'A';
    }

    int invP[2][2];
    if (!inverseMatrix(P, invP)) {
        printf("Plaintext matrix not invertible.\n");
        return;
    }

    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            key[i][j] = 0;
            for (int k=0; k<2; k++)
                key[i][j] += C[i][k] * invP[k][j];
            key[i][j] = (key[i][j] % 26 + 26) % 26;
        }
    }
}

int main() {
    int key[2][2] = {{3, 3}, {2, 5}}; // Original key matrix
    char pt[] = "HELP";
    char ct[5];

    // Encrypt
    encrypt(pt, ct, key);
    encrypt(pt + 2, ct + 2, key);
    ct[4] = '\0';

    printf("Original Plaintext: %s\n", pt);
    printf("Encrypted Ciphertext: %s\n", ct);

    // Known Plaintext Attack
    int recoveredKey[2][2];
    knownPlaintextAttack(pt, ct, recoveredKey);

    printf("Recovered Key Matrix from Known Plaintext Attack:\n");
    printf("%d %d\n", recoveredKey[0][0], recoveredKey[0][1]);
    printf("%d %d\n", recoveredKey[1][0], recoveredKey[1][1]);

    // Decrypt
    char decrypted[5];
    decrypt(ct, decrypted, key);
    decrypt(ct + 2, decrypted + 2, key);
    decrypted[4] = '\0';
    printf("Decrypted Text: %s\n", decrypted);

    return 0;
}
