#include "murmur3.h"
#include "common.h"

inline uint32_t
ROTL32(uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

unsigned int
bcn_murmur3(const unsigned char *data, size_t len, unsigned int seed) {
  uint32_t h1 = seed;

  if (len > 0) {
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = len / 4;

    const uint8_t *blocks = &data[0] + nblocks * 4;

    for (int i = -nblocks; i; i++) {
      uint32_t k1 = READU32(blocks + i * 4);

      k1 *= c1;
      k1 = ROTL32(k1, 15);
      k1 *= c2;

      h1 ^= k1;
      h1 = ROTL32(h1, 13);
      h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t* tail = (const uint8_t*)(&data[0] + nblocks * 4);

    uint32_t k1 = 0;

    switch (len & 3) {
      case 3:
        k1 ^= tail[2] << 16;
      case 2:
        k1 ^= tail[1] << 8;
      case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    }
  }

  h1 ^= len;
  h1 ^= h1 >> 16;
  h1 *= 0x85ebca6b;
  h1 ^= h1 >> 13;
  h1 *= 0xc2b2ae35;
  h1 ^= h1 >> 16;

  return h1;
}
