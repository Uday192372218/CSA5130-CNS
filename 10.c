#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Fixed 5x5 Playfair matrix (as per your input)
char matrix[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};

// Get position of a character in the matrix
void getPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';  // Replace J with I
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
}

// Prepares the plaintext into digraphs with 'X' filler
void prepareText(const char *input, char *output) {
    int i = 0, j = 0;
    char prev = '\0';
    while (input[i]) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            if (ch == 'J') ch = 'I';

            if (j > 0 && output[j - 1] == ch) {
                output[j++] = 'X'; // Insert X if same letter repeated
            }
            output[j++] = ch;
        }
        i++;
    }
    if (j % 2 != 0)
        output[j++] = 'X'; // Pad with X if odd
    output[j] = '\0';
}

// Encrypts a pair of characters
void encryptPair(char a, char b) {
    int r1, c1, r2, c2;
    getPosition(a, &r1, &c1);
    getPosition(b, &r2, &c2);

    if (r1 == r2) {
        printf("%c%c", matrix[r1][(c1 + 1) % SIZE], matrix[r2][(c2 + 1) % SIZE]);
    } else if (c1 == c2) {
        printf("%c%c", matrix[(r1 + 1) % SIZE][c1], matrix[(r2 + 1) % SIZE][c2]);
    } else {
        printf("%c%c", matrix[r1][c2], matrix[r2][c1]);
    }
}

// Encrypt full message
void encrypt(const char *plaintext) {
    char prepared[300];
    prepareText(plaintext, prepared);

    printf("\nPrepared text (digraphs):\n");
    for (int i = 0; prepared[i]; i += 2)
        printf("%c%c ", prepared[i], prepared[i+1]);

    printf("\n\nEncrypted text:\n");
    for (int i = 0; prepared[i]; i += 2)
        encryptPair(prepared[i], prepared[i + 1]);

    printf("\n");
}

int main() {
    const char *message = "Must see you over Cadogan West. Coming at once.";

    printf("Original Message:\n%s\n", message);
    encrypt(message);

    return 0;
}
