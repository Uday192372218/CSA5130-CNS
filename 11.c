#include <stdio.h>
#include <math.h>

int main() {
    double total_perms = 1;
    for (int i = 1; i <= 25; i++) {
        total_perms *= i;
    }

    double log2_total = log2(total_perms);
    double effective_keys = total_perms / 600.0;  // estimate
    double log2_effective = log2(effective_keys);

    printf("Total possible Playfair keys (25!):\n≈ %.2e ≈ 2^%.0f\n", total_perms, log2_total);
    printf("Effectively unique Playfair keys (estimated):\n≈ %.2e ≈ 2^%.0f\n", effective_keys, log2_effective);

    return 0;
}
