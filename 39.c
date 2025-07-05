#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT 1000

int get_letter_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

char decrypt_char(char c, int key) {
    int idx = get_letter_index(c);
    if (idx == -1) return c;
    return ((idx - key + 26) % 26) + 'A';
}

void decrypt_text(char *cipher, char *plain, int key) {
    int i;
    for (i = 0; cipher[i]; i++) {
        char c = cipher[i];
        int idx = get_letter_index(c);
        if (idx != -1)
            plain[i] = decrypt_char(c, key);
        else
            plain[i] = c;
    }
    plain[i] = '\0';
}

int most_frequent_letter(char *text) {
    int freq[26] = {0}, max = 0, index = 0;
    for (int i = 0; text[i]; i++) {
        int idx = get_letter_index(text[i]);
        if (idx != -1) {
            freq[idx]++;
            if (freq[idx] > max) {
                max = freq[idx];
                index = idx;
            }
        }
    }
    return index;
}

int main() {
    char ciphertext[MAX_TEXT], decrypted[MAX_TEXT];
    int top_n;

    printf("Enter ciphertext (A–Z or a–z only): ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("Enter how many top guesses to show: ");
    scanf("%d", &top_n);

    int most_freq = most_frequent_letter(ciphertext);

    for (int guess = 0; guess < top_n; guess++) {
        int key = (most_freq - get_letter_index('E') + guess + 26) % 26;
        decrypt_text(ciphertext, decrypted, key);
        printf("\nGuess #%d (key = %d):\n%s\n", guess + 1, key, decrypted);
    }

    return 0;
}
