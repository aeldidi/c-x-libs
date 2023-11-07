#ifndef HASH_H
#define HASH_H
#include <inttypes.h>
#include <stddef.h>

// calculates the fnv-1a hash of the string str.
uint64_t fnv_1a_str(const char* str);

// calculates the fnv-1a hash of the first buf_len bytes of the buffer buf.
uint64_t fnv_1a(const size_t buf_len, const uint8_t* buf);

#endif // HASH_H
