#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 1000
#define ALPHABET_LEN 26

const char english_freq[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} FreqEntry;

int compare_freq(const void *a, const void *b) {
    FreqEntry *fa = (FreqEntry *)a;
    FreqEntry *fb = (FreqEntry *)b;
    return fb->count - fa->count;
}

void count_frequencies(const char *text, FreqEntry *freq_table) {
    for (int i = 0; i < ALPHABET_LEN; i++) {
        freq_table[i].letter = 'A' + i;
        freq_table[i].count = 0;
    }

    for (int i = 0; text[i]; i++) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {
            freq_table[c - 'A'].count++;
        }
    }
}

void apply_mapping(const char *cipher, char *plain, char *mapping) {
    for (int i = 0; cipher[i]; i++) {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
            plain[i] = mapping[cipher[i] - 'A'];
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[strlen(cipher)] = '\0';
}

void frequency_attack(const char *ciphertext, int top_n) {
    FreqEntry freq_table[ALPHABET_LEN];
    count_frequencies(ciphertext, freq_table);
    qsort(freq_table, ALPHABET_LEN, sizeof(FreqEntry), compare_freq);

    for (int k = 0; k < top_n && k < ALPHABET_LEN; k++) {
        char mapping[ALPHABET_LEN];
        for (int i = 0; i < ALPHABET_LEN; i++) {
            mapping[freq_table[i].letter - 'A'] = english_freq[(i + k) % ALPHABET_LEN];
        }

        char result[MAX_TEXT];
        apply_mapping(ciphertext, result, mapping);
        printf("\nGuess #%d:\n%s\n", k + 1, result);
    }
}

int main() {
    char ciphertext[MAX_TEXT];
    int top_n;

    printf("Enter ciphertext (uppercase letters only): ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("Enter number of top guesses to display: ");
    scanf("%d", &top_n);

    frequency_attack(ciphertext, top_n);
    return 0;
}
