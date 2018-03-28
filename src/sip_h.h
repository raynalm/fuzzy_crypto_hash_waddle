#include <stdint.h>

uint64_t siphash_2_4(uint64_t k[2], uint8_t *msg, unsigned mlen);

void sipround(uint64_t *v0, uint64_t *v1, uint64_t *v2, uint64_t *v3);

void left_rotation(uint64_t *v, int n);
