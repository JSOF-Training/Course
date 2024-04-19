#include <stdint.h>

static inline uint64_t rotl64 (uint64_t n, unsigned int c);
static inline uint64_t rotr64 (uint64_t n, unsigned int c);
static inline uint64_t roll64(uint64_t n, uint64_t pattern);
static inline uint64_t unroll64(uint64_t n, uint64_t pattern);
static inline uint64_t rolln64(uint64_t n, uint64_t pattern, unsigned int c);
static inline uint64_t unrolln64(uint64_t n, uint64_t pattern, unsigned int c);


static inline uint64_t rotl64 (uint64_t n, unsigned int c)
{
  const unsigned int mask = 63;

  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

static inline uint64_t rotr64 (uint64_t n, unsigned int c)
{
  const unsigned int mask = 63;

  c &= mask;
  return (n>>c) | (n<<( (-c)&mask ));
}

static inline uint64_t roll64(uint64_t n, uint64_t pattern)
{
  const uint64_t mask = (uint64_t) 1 << 63;
  if (n & mask)
    return (n << 1) ^ pattern;
  return (n << 1);
}

static inline uint64_t unroll64(uint64_t n, uint64_t pattern)
{
  const uint64_t mask = (uint64_t) 1 << 63;
  if (n & 1)
    return ((n ^ pattern) >> 1) | mask;
  return (n >> 1);
}

static inline uint64_t rolln64(uint64_t n, uint64_t pattern, unsigned int c)
{
  for (unsigned int i=0; i<c; i++)
    n = roll64(n, pattern);
  return n;
}

static inline uint64_t unrolln64(uint64_t n, uint64_t pattern, unsigned int c)
{
  for (unsigned int i=0; i<c; i++)
    n = unroll64(n, pattern);
  return n;
}
