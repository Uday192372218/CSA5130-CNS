#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TEXT_LEN 1000
#define ALPHABET_SIZE 26

double english_freq[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749,
    7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

typedef struct {
    int key;
    char text[MAX_TEXT_LEN];
    double score;
} Decryption;

void decrypt(char *ciphertext, char *output, int key) {
    int i;
    for (i = 0; ciphertext[i] != '\0'; i++) {
        char c = ciphertext[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            output[i] = ((c - base - key + ALPHABET_SIZE) % ALPHABET_SIZE) + base;
        } else {
            output[i] = c;
        }
    }
    output[i] = '\0';
}

double calculate_score(char *text) {
    int letter_counts[26] = {0};
    int total_letters = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            letter_counts[tolower(text[i]) - 'a']++;
            total_letters++;
        }
    }
    if (total_letters == 0) return 1e9;
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed = (double)letter_counts[i] * 100 / total_letters;
        double expected = english_freq[i];
        score += ((observed - expected) * (observed - expected)) / expected;
    }
    return score;
}

int compare(const void *a, const void *b) {
    double diff = ((Decryption *)a)->score - ((Decryption *)b)->score;
    return (diff > 0) - (diff < 0);
}

int main() {
    char ciphertext[MAX_TEXT_LEN];
    int topN;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT_LEN, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0;

    printf("Enter number of top results to display: ");
    scanf("%d", &topN);

    Decryption results[ALPHABET_SIZE];

    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decrypt(ciphertext, results[key].text, key);
        results[key].key = key;
        results[key].score = calculate_score(results[key].text);
    }

    qsort(results, ALPHABET_SIZE, sizeof(Decryption), compare);

    printf("\nTop %d possible plaintexts:\n", topN);
    for (int i = 0; i < topN && i < ALPHABET_SIZE; i++) {
        printf("Key: %2d | Score: %6.2f | Plaintext: %s\n",
            results[i].key, results[i].score, results[i].text);
    }

    return 0;
}
