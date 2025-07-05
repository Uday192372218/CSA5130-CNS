#include <stdio.h>
#include <string.h>

#define MOD 26

int mod_inverse(int a) {
    a = a % MOD;
    for (int x = 1; x < MOD; x++) {
        if ((a * x) % MOD == 1)
            return x;
    }
    return -1;
}

int determinant(int key[2][2]) {
    return (key[0][0]*key[1][1] - key[0][1]*key[1][0]) % MOD;
}

void get_inverse_key(int key[2][2], int inverse[2][2]) {
    int det = determinant(key);
    int det_inv = mod_inverse(det);
    if (det_inv == -1) {
        printf("Key matrix is not invertible!\n");
        return;
    }

    inverse[0][0] =  key[1][1] * det_inv % MOD;
    inverse[0][1] = -key[0][1] * det_inv % MOD;
    inverse[1][0] = -key[1][0] * det_inv % MOD;
    inverse[1][1] =  key[0][0] * det_inv % MOD;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            inverse[i][j] = (inverse[i][j] + MOD) % MOD;
        }
}

void multiply(int key[2][2], int input[2], int output[2]) {
    for (int i = 0; i < 2; i++) {
        output[i] = 0;
        for (int j = 0; j < 2; j++) {
            output[i] += key[i][j] * input[j];
        }
        output[i] %= MOD;
    }
}

void text_to_vector(char *text, int vec[2]) {
    vec[0] = text[0] - 'A';
    vec[1] = text[1] - 'A';
}

void vector_to_text(int vec[2], char *text) {
    text[0] = vec[0] + 'A';
    text[1] = vec[1] + 'A';
    text[2] = '\0';
}

int main() {
    char plaintext[3], ciphertext[3], decrypted[3];
    int key[2][2], key_inv[2][2];
    int pvec[2], cvec[2], dvec[2];

    printf("Enter 2x2 key matrix (integers):\n");
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            scanf("%d", &key[i][j]);

    printf("Enter 2-letter UPPERCASE plaintext (e.g., HE): ");
    scanf("%s", plaintext);

    text_to_vector(plaintext, pvec);
    multiply(key, pvec, cvec);
    vector_to_text(cvec, ciphertext);

    printf("Encrypted text: %s\n", ciphertext);

    get_inverse_key(key, key_inv);
    multiply(key_inv, cvec, dvec);
    vector_to_text(dvec, decrypted);

    printf("Decrypted text: %s\n", decrypted);

    return 0;
}
