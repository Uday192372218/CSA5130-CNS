#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Simple hash function for simulation (not cryptographically secure)
unsigned int simple_hash(const char *msg) {
    unsigned int h = 0;
    for (int i = 0; msg[i]; i++)
        h = (h * 31 + msg[i]) % 9973;  // prime mod
    return h;
}

// DSA mock signature: sig = (hash(msg) * k) mod q
unsigned int dsa_sign(const char *msg, int q) {
    int k = rand() % (q - 1) + 1;  // random k (1 ≤ k < q)
    unsigned int hash = simple_hash(msg);
    return (hash * k) % q;
}

// RSA mock signature: sig = hash(msg)^d mod n
unsigned int rsa_sign(const char *msg, int d, int n) {
    unsigned int hash = simple_hash(msg);
    unsigned int sig = 1;
    for (int i = 0; i < d; i++)
        sig = (sig * hash) % n;
    return sig;
}

int main() {
    srand(time(NULL));
    const char *message = "HELLO";

    int q = 9973;       // large prime for DSA modulus
    int rsa_d = 7;      // private exponent
    int rsa_n = 3233;   // modulus

    printf("Message: %s\n", message);
    printf("\nDSA Signatures (with random k):\n");
    for (int i = 0; i < 3; i++) {
        unsigned int sig = dsa_sign(message, q);
        printf("  Signature %d: %u\n", i + 1, sig);
    }

    printf("\nRSA Signatures (same every time):\n");
    for (int i = 0; i < 3; i++) {
        unsigned int sig = rsa_sign(message, rsa_d, rsa_n);
        printf("  Signature %d: %u\n", i + 1, sig);
    }

    return 0;
}
