#include <stdio.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main() {
    long long n = 3599;         // n = p × q (59 × 61)
    long long e = 31;           // Public key
    long long m = 122;          // Let's say this plaintext block has common factor with n

    long long factor = gcd(m, n);

    if (factor > 1 && factor < n) {
        printf("Found non-trivial common factor with n: %lld\n", factor);
        long long p = factor;
        long long q = n / p;
        long long phi = (p - 1) * (q - 1);

        long long d = 0;
        for (long long i = 1; i < phi; i++) {
            if ((i * e) % phi == 1) {
                d = i;
                break;
            }
        }

        printf("Recovered primes: p = %lld, q = %lld\n", p, q);
        printf("Private key (d) = %lld\n", d);
    } else {
        printf("Plaintext does not share a common factor with n. RSA still secure.\n");
    }

    return 0;
}
