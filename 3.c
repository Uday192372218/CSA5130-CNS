#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];

// Function to generate key square
void generateKeyMatrix(char key[]) {
    int i, j, k = 0, flag, used[26] = {0};

    // Fill key first
    for (i = 0; i < strlen(key); i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';
        if (isalpha(ch) && !used[ch - 'A']) {
            matrix[k / SIZE][k % SIZE] = ch;
            used[ch - 'A'] = 1;
            k++;
        }
    }

    // Fill remaining letters
    for (i = 0; i < 26; i++) {
        if (i == ('J' - 'A')) continue; // Skip J
        if (!used[i]) {
            matrix[k / SIZE][k % SIZE] = 'A' + i;
            used[i] = 1;
            k++;
        }
    }
}

// Function to prepare plaintext into digraphs
void prepareText(char input[], char output[]) {
    int i, j = 0;
    for (i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]))
            continue;

        char ch = toupper(input[i]);
        if (ch == 'J') ch = 'I';
        output[j++] = ch;
    }
    output[j] = '\0';

    // Insert X between repeating letters
    for (i = 0; i < j - 1; i += 2) {
        if (output[i] == output[i + 1]) {
            memmove(&output[i + 2], &output[i + 1], strlen(&output[i + 1]) + 1);
            output[i + 1] = 'X';
            j++;
        }
    }
    // If odd length, append X
    if (j % 2 != 0) {
        output[j++] = 'X';
        output[j] = '\0';
    }
}

// Find position of character in matrix
void findPosition(char ch, int *row, int *col) {
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
}

// Encrypt pair of letters
void encryptPair(char a, char b, char *out1, char *out2) {
    int row1, col1, row2, col2;
    findPosition(a, &row1, &col1);
    findPosition(b, &row2, &col2);

    if (row1 == row2) { // Same row
        *out1 = matrix[row1][(col1 + 1) % SIZE];
        *out2 = matrix[row2][(col2 + 1) % SIZE];
    }
    else if (col1 == col2) { // Same column
        *out1 = matrix[(row1 + 1) % SIZE][col1];
        *out2 = matrix[(row2 + 1) % SIZE][col2];
    }
    else { // Rectangle swap
        *out1 = matrix[row1][col2];
        *out2 = matrix[row2][col1];
    }
}

void encrypt(char plaintext[]) {
    int i;
    char prepared[100], encrypted[100];
    prepareText(plaintext, prepared);

    printf("\nEncrypted text: ");
    for (i = 0; i < strlen(prepared); i += 2) {
        char a, b;
        encryptPair(prepared[i], prepared[i + 1], &a, &b);
        printf("%c%c", a, b);
    }
    printf("\n");
}

// For debugging (display matrix)
void displayMatrix() {
    int i, j;
    printf("\nKey Matrix:\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char key[100], plaintext[100];

    printf("Enter the keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0;

    generateKeyMatrix(key);
    displayMatrix();
    encrypt(plaintext);

    return 0;
}
