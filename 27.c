#include <stdio.h>
#include <math.h>

// Simple modular exponentiation
long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

int main() {
    long long e = 17;          // Public exponent
    long long n = 3233;        // Public modulus

    printf("Encrypting A–Z using RSA (e = %lld, n = %lld):\n\n", e, n);
    for (int i = 0; i < 26; i++) {
        long long cipher = mod_exp(i, e, n);
        printf("Plaintext %c (%2d) → Ciphertext: %4lld\n", 'A' + i, i, cipher);
    }

    // Attacker's lookup attack
    printf("\n🔓 Attacker's Decryption Table (Precomputed):\n");
    long long lookup[26];
    for (int i = 0; i < 26; i++) {
        lookup[i] = mod_exp(i, e, n);
    }

    long long target;
    printf("\nEnter intercepted ciphertext value to break: ");
    scanf("%lld", &target);

    for (int i = 0; i < 26; i++) {
        if (lookup[i] == target) {
            printf("Attacker cracked it: Cipher %lld = Plaintext '%c'\n", target, 'A' + i);
            break;
        }
    }

    return 0;
}
