#include <stdio.h>
#include <math.h>

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    long long p = 59, q = 61;
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    long long e = 31;
    long long d = 3371;

    long long plaintext;
    printf("Enter plaintext (as number < %lld): ", n);
    scanf("%lld", &plaintext);

    long long ciphertext = mod_exp(plaintext, e, n);
    printf("Encrypted: %lld\n", ciphertext);

    long long decrypted = mod_exp(ciphertext, d, n);
    printf("Decrypted: %lld\n", decrypted);

    return 0;
}
