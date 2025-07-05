#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    long long q = 353;      // A large prime number
    long long a = 3;        // Primitive root modulo q

    long long xA = 97;      // Alice's secret
    long long xB = 233;     // Bob's secret

    long long A = mod_exp(a, xA, q);  // Alice sends A = a^xA mod q
    long long B = mod_exp(a, xB, q);  // Bob sends B = a^xB mod q

    long long secretA = mod_exp(B, xA, q);  // Alice computes key
    long long secretB = mod_exp(A, xB, q);  // Bob computes key

    printf("Public values: q = %lld, a = %lld\n", q, a);
    printf("Alice's public key: %lld\n", A);
    printf("Bob's public key: %lld\n", B);
    printf("Shared secret (Alice): %lld\n", secretA);
    printf("Shared secret (Bob):   %lld\n", secretB);

    return 0;
}
