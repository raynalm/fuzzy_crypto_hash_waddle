# fuzzy_crypto_hash_waddle
Crypto lab about sip hash

## Question 2
 + The key space of sip_hash_2_4 has a size of 2^128. If we consider that as the authors claim, this hash function prvodes maximum PRF security, then it would take an average 2^127 attacks to recover the key. So we can consider that the key space is just big enough under these assumptions. But to be fairly secure, a good key space would nowadays have to be bigger, 2^256 for instance.
 + This function cannot be considered to be collision resistant, since the if we consider siphash(k , ., .), it is a function of 2^128 -> 2^64. Therefore it is not injective, and we can expect an average of 2^64 distinct keys to posses the same siphash2_4.

## Question 3
 + An easy way to build an injective mapping is to truncate the inputs to their  least significant bytes. Then, the injectivity is trivial, and the process to obtain them is very simple.