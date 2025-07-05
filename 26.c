#include <stdio.h>
#include <stdlib.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long mod_inverse(long long e, long long phi) {
    for (long long d = 1; d < phi; d++) {
        if ((d * e) % phi == 1)
            return d;
    }
    return -1;
}

int main() {
    long long p = 61, q = 53;
    long long n = p * q;         // 61 * 53 = 3233
    long long phi = (p - 1) * (q - 1); // 60 * 52 = 3120

    long long e1 = 17;
    long long d1 = mod_inverse(e1, phi);

    printf("Original Public Key: (e = %lld, n = %lld)\n", e1, n);
    printf("Original Private Key: d = %lld\n", d1);

    printf("\nOops! Bob leaked private key d = %lld\n", d1);

    long long e2 = 31;
    long long d2 = mod_inverse(e2, phi);
    printf("\nBob now generates new e = %lld → New d = %lld\n", e2, d2);

    printf("\n⚠ Even if Bob changes (e,d), attacker already knows φ(n) = %lld\n", phi);
    printf("→ Attacker can compute any new private key d.\n");
    printf("→ Also factor n = %lld as p = %lld, q = %lld\n", n, p, q);

    return 0;
}
