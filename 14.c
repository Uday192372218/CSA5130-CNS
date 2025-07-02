#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

void encrypt(char plaintext[], int key[], char ciphertext[]) {
    int len = strlen(plaintext);
    for (int i = 0, j = 0; i < len; i++) 
	{
        if (plaintext[i] == ' ') {
            ciphertext[i] = ' ';
        } else {
            char p = tolower(plaintext[i]) - 'a';
            int shift = key[j++] % 26;
            ciphertext[i] = (p + shift) % 26 + 'a';
        }
    }
    ciphertext[len] = '\0';
}

void find_key(char plaintext[], char ciphertext[], int key[]) {
    int len = strlen(plaintext);
    for (int i = 0, j = 0; i < len; i++) {
        if (plaintext[i] == ' ') {
            continue;
        } else {
            int p = tolower(plaintext[i]) - 'a';
            int c = tolower(ciphertext[i]) - 'a';
            key[j++] = (c - p + 26) % 26;
        }
    }
}

void print_key(int key[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", key[i]);
    }
    printf("\n");
}

int main() {
    char plaintext1[] = "send more money";
    int key1[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    char ciphertext1[MAX];

    encrypt(plaintext1, key1, ciphertext1);

    printf("Part (a):\n");
    printf("Plaintext : %s\n", plaintext1);
    printf("Key       : ");
    print_key(key1, 13);
    printf("Ciphertext: %s\n\n", ciphertext1);

    char plaintext2[] = "cash not needed";
    char ciphertext2[] = "lfzw bzi zwvmdm";
    int key2[20];

    find_key(plaintext2, ciphertext2, key2);

    printf("Part (b):\n");
    printf("Plaintext : %s\n", plaintext2);
    printf("Ciphertext: %s\n", ciphertext2);
    printf("Derived Key: ");
    print_key(key2, 14);

    return 0;
}
