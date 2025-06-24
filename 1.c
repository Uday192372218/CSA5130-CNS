#include <stdio.h>
#include <ctype.h>
#include <string.h>

void encrypt(char *text, int k) {
    int i;
    char ch;
    for (i = 0; text[i] != '\0'; i++) {
        ch = text[i];

        if (isupper(ch)) {
            ch = ((ch - 'A' + k) % 26) + 'A';
        } else if (islower(ch)) {
            ch = ((ch - 'a' + k) % 26) + 'a';
        }
        text[i] = ch;
    }
}

int main() {
    char text[1000];
    int k;
    printf("Enter a message to encrypt: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter shift value (1-25): ");
    scanf("%d", &k);
    if (k < 1 || k > 25) {
        printf("Invalid shift value. Please enter a value between 1 and 25.\n");
        return 1;
    }

    encrypt(text, k);

    printf("Encrypted message: %s\n", text);

    return 0;
}
