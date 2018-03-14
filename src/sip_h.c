#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "sip_h.h"

#define C0 0x736f6d6570736575
#define C1 0x646f72616e646f6d
#define C2 0x6c7967656e657261
#define C3 0x7465646279746573

#define C 2
#define D 4



uint64_t siphash_2_4(uint64_t k[2], uint8_t *msg, unsigned mlen) {
/* initialisation */
  printf("k[0] : 0x%jx, k[1] : 0x%jx\n", k[0], k[1]);
  uint64_t v0, v1, v2, v3;
  v0 = k[0] ^ C0;
  v1 = k[1] ^ C1;
  v2 = k[0] ^ C2;
  v3 = k[1] ^ C3;
  printf("v0 :0x%jx; v1 : 0x%jx; v2 : 0x%jx; v3 : 0x%jx\n", v0, v1, v2, v3);
/* compression */
  size_t w = ( (mlen+1)%8 == 0 ? (mlen+1)/8 : (mlen+1)/8 +1 );
  printf("w : %zu\n", w);
  uint64_t *m = malloc(w*sizeof(uint64_t));
  uint8_t *m_i;

  /* first w-1 words */
  for (int i = 0; i < w-1; i++) {
    m[i] = *((uint64_t *)msg+i);
  }

  /* last word */
  m_i = (uint8_t *)(m + w-1);
  for (int i = 0; i < 7; i++) {
    if (((w-1)*8+i) < mlen) {
      m_i[i] = msg[(w-1)*8 +i];
    } else {
      m_i[i] = 0;
    }
  }
  m_i[7] = (uint8_t) (mlen % 256);

  for (int i = 0; i < w; i++) {
    printf("m[%d] : 0x%jx ", i, m[i]);
  }
  printf("\n");

  /* process the m_i */
  for (int i = 0; i < w; i++) {
    v3 ^= m[i];
    printf("v3^m[%d] : 0x%jx\n", i, v3);
    for (int j = 0; j < C; j++) {
      sipround(&v0, &v1, &v2, &v3);
    }
    v0 ^= m[i];
    printf("v0 :0x%jx; v1 : 0x%jx; v2 : 0x%jx; v3 : 0x%jx\n", v0, v1, v2, v3);
  }

    /* finalisation */
  v2 ^= 0xff;

  for (int i =0; i < D; i++) {
    sipround(&v0, &v1, &v2, &v3);
  }

  return (v0 ^ v1 ^ v2 ^ v3);
}

void sipround(uint64_t *v0, uint64_t *v1, uint64_t *v2, uint64_t *v3) {
  (*v0) += (*v1);
  (*v2) += (*v3);
  left_rotation(v1, 13);
  left_rotation(v3, 16);
  (*v1) ^= (*v0);
  (*v3) ^= (*v2);
  left_rotation(v0, 32);
  (*v2) += (*v1);
  (*v0) += (*v3);
  left_rotation(v1, 17);
  left_rotation(v3, 21);
  (*v1) ^= (*v2);
  (*v3) ^= (*v0);
  left_rotation(v2, 32);
}

void left_rotation(uint64_t *v, int n) {
  uint64_t v1 = *v >> (64-n), v2 = *v << n;
  *v = v1 + v2;
}


int main() {
  uint64_t k[2] = {0x0706050403020100, 0x0f0e0d0c0b0a0908};
  uint8_t msg[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  unsigned mlen = 15;
  printf("0x%jx \n", siphash_2_4(k, msg, mlen));
  return 0;
}
