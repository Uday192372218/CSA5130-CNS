#include <stdio.h>
#include <string.h>

#define SIZE 1000

char ciphertext[] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83\
(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*\
;4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81\
(‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";

int freq[256] = {0};
char mapping[256] = {0};

// Function to count frequency
void count_frequency(char *text) {
    for (int i = 0; text[i]; i++) {
        unsigned char ch = text[i];
        freq[ch]++;
    }
    printf("\nCharacter Frequencies:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            printf("'%c' (%d): %d\n", i, i, freq[i]);
        }
    }
}

// Function to apply mapping
void decrypt_text(char *text) {
    printf("\nDecrypted Message (partial mapping):\n");
    for (int i = 0; text[i]; i++) {
        unsigned char ch = text[i];
        if (mapping[ch] != 0)
            printf("%c", mapping[ch]);
        else
            printf("%c", ch); // unknown character
    }
    printf("\n");
}

int main() {
    printf("Original Ciphertext:\n%s\n", ciphertext);

    count_frequency(ciphertext);

    // STEP 1: Based on frequency analysis (done manually or using above freq data)
    // Suppose we assume:
    // '‡' = 'E'  (very frequent, likely 'e')
    // '*' = 'T'  (next frequent, likely 't')
    // ';' = 'H'  (part of 'THE')
    // '8' = 'O'
    // '4' = 'R'
    // '5' = 'N'
    // '6' = 'D'
    // ')' = 'A'
    // '†' = 'S'
    // ':' = 'U'
    // '?' = 'Y'
    // '.' = 'M'
    // '0' = 'C'
    // '3' = 'K'
    // '9' = 'I'
    // '1' = 'B'
    // '2' = 'G'
    // '—' = 'F'
    // '¶' = 'V'
    // ']' = 'P'
    // '(' = 'L'

    mapping['‡'] = 'E';
    mapping['*'] = 'T';
    mapping[';'] = 'H';
    mapping['8'] = 'O';
    mapping['4'] = 'R';
    mapping['5'] = 'N';
    mapping['6'] = 'D';
    mapping[')'] = 'A';
    mapping['†'] = 'S';
    mapping[':'] = 'U';
    mapping['?'] = 'Y';
    mapping['.'] = 'M';
    mapping['0'] = 'C';
    mapping['3'] = 'K';
    mapping['9'] = 'I';
    mapping['1'] = 'B';
    mapping['2'] = 'G';
    mapping['—'] = 'F';
    mapping['¶'] = 'V';
    mapping[']'] = 'P';
    mapping['('] = 'L';

    decrypt_text(ciphertext);

    return 0;
}
