#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sip_h.h"
#include "search_tree.h"


#define C0 0x736f6d6570736575
#define C1 0x646f72616e646f6d
#define C2 0x6c7967656e657261
#define C3 0x7465646279746573
#define C 2
#define D 4
#define NB_ITER 1000

#define UNSAFE_COLLISIONS_SEARCH


extern node_list head, current;
uint32_t some_uint32_t;


uint64_t siphash_2_4(uint64_t k[2], uint8_t *msg, unsigned mlen) {
  /* initialisation */
  uint64_t v0, v1, v2, v3;
  v0 = k[0] ^ C0;
  v1 = k[1] ^ C1;
  v2 = k[0] ^ C2;
  v3 = k[1] ^ C3;


  size_t w = ( (mlen+1)%8 == 0 ? (mlen+1)/8 : (mlen+1)/8 +1 );
  uint64_t *m = malloc(w*sizeof(uint64_t));
  uint8_t *m_i;

  /* first w-1 words */
  for (size_t i = 0; i < w-1; i++) {
    m[i] = *((uint64_t *)msg+i);
  }

  /* last word */
  m_i = (uint8_t *)(m + w-1); //m[w-1][i]
  for (int i = 0; i < 7; i++) {
    if (((w-1)*8+i) < mlen) {
      m_i[i] = msg[(w-1)*8 +i];
    } else {
      m_i[i] = 0;
    }
  }
  m_i[7] = (uint8_t) (mlen % 256);



  /* compression */
  for (size_t i = 0; i < w; i++) {
    v3 ^= m[i];
    for (size_t j = 0; j < C; j++) {
      sipround(&v0, &v1, &v2, &v3);
    }
    v0 ^= m[i];
  }

    /* finalisation */
  v2 ^= 0xff;
  for (int i =0; i < D; i++) {
    sipround(&v0, &v1, &v2, &v3);
  }

  free(m);
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

uint32_t sip_hash_fix32(uint32_t k, uint32_t m){
  uint64_t k128[2] = {(uint64_t)k, (uint64_t)k};
  //uint8_t *m8 = malloc(4*sizeof(uint8_t));
  //*((uint32_t *)m8) = m;
  uint32_t result = (uint32_t)siphash_2_4(k128, (uint8_t *)&m, 4);
  //free(m8);
  return result;
}


void coll_search(uint32_t k, uint32_t (*fun)(uint32_t, uint32_t)) {
  uint64_t iter = 0;
  head = init_node_list();
  current = head;
  tree t = init_node();

#ifdef UNSAFE_COLLISIONS_SEARCH
  while (!is_in(t, (*fun)(k, iter++)));
#else
  while (!is_in(t, (*fun)(k, iter++)) && iter < ITER_MAX);
#endif
  printf("%jd ", iter-1);
  free_it_all();
}

int main(int argc, char *argv[]) {
  /* uint64_t k[2] = {0x0706050403020100, 0x0f0e0d0c0b0a0908}; */
  /* uint8_t msg[] = {0, 1, 2, 3, 4, 5, 6, 7};//, 8, 9, 10, 11, 12, 13, 14, 15}; */
  /* //uint8_t *msg = NULL; */
  /* unsigned mlen = 8; */
  /* //uint64_t k[2] = {0}; */

  size_t nb_iter;
  if (argc < 2){
    nb_iter = NB_ITER;
  } else {
    nb_iter = atoi(argv[1]);
  }

  srand(time(NULL));
  some_uint32_t = (uint32_t) rand();

  for (size_t i = 0; i < nb_iter; i++){
    coll_search(rand(), sip_hash_fix32);
  }
  return 0;
}
