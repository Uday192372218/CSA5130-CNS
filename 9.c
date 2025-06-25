#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];

// Function to prepare the 5x5 Playfair matrix
void generateMatrix(char key[]) {
    int used[26] = {0};
    int i, j, k = 0;

    used['J' - 'A'] = 1; // J is merged with I

    for (i = 0; i < strlen(key); i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A']) {
            matrix[k / SIZE][k % SIZE] = ch;
            used[ch - 'A'] = 1;
            k++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (!used[i]) {
            matrix[k / SIZE][k % SIZE] = 'A' + i;
            used[i] = 1;
            k++;
        }
    }
}

// Get position of character in matrix
void getPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
}

// Decrypt pair
void decryptPair(char a, char b) {
    int r1, c1, r2, c2;
    getPosition(a, &r1, &c1);
    getPosition(b, &r2, &c2);

    if (r1 == r2) {
        printf("%c%c", matrix[r1][(c1 + 4) % 5], matrix[r2][(c2 + 4) % 5]);
    } else if (c1 == c2) {
        printf("%c%c", matrix[(r1 + 4) % 5][c1], matrix[(r2 + 4) % 5][c2]);
    } else {
        printf("%c%c", matrix[r1][c2], matrix[r2][c1]);
    }
}

// Decrypt full ciphertext
void decrypt(char *cipher) {
    int len = strlen(cipher);
    for (int i = 0; i < len; i += 2) {
        char a = toupper(cipher[i]);
        char b = toupper(cipher[i + 1]);
        if (isalpha(a) && isalpha(b))
            decryptPair(a, b);
    }
    printf("\n");
}

int main() {
    char key[] = "MONARCHY"; // You can change this to the actual key
    char ciphertext[] =
        "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPO"
        "BOTEIZONTXBYBNTGONEYCUZWRGDSONSXBOUYWRHEBAAHYUSEDQ";

    generateMatrix(key);

    printf("Decrypted message:\n");
    decrypt(ciphertext);

    return 0;
}
