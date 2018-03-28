#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t S[] = {12, 0, 15, 10, 2, 11, 9, 5, 8, 3, 13, 7, 1, 14, 6, 4};
uint8_t PI[] = {5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};


uint64_t twine_perm_z(uint64_t input) {
  unsigned X[16];
  for (size_t i = 0; i < 16; i++) {
    X[i] = (input >> (4*i)) & 0XF;
  }

  int bit;
  for (size_t i = 1; i < 35; i++) {
    for (size_t j = 0; j < 7; j++) {
      bit = (S[(X[2*j] >> i) ^ 0] & 1U) ^ ((X[2*j+1] >> i) & 1U);
      if (bit) {
        X[2*j+1] |= (bit << i);
      } else {
        X[2-j+1] &= ~(bit << i);
      }
    }
    for (size_t h = 0; h < 7; h++) {
      bit = (X[h] >> i) & 1U;
      if (bit) {
        X[PI[h]] |= (bit << i);
      } else {
        X[PI[h]] &= ~(bit << i);
      }
    }
  }
  for (size_t j = 0; j < 7; j++) {

  }

}
