#ifndef SLICE_H
#define SLICE_H
#include <inttypes.h>
#include <stddef.h>

#include "c.eldidi.org/x/arena"

// A slice is any struct which has a len, cap, and data member laid out like
// so:
struct Slice {
	void*  data;
	size_t len;
	size_t cap;
} Slice;

#define slice_push(arena, s)                                                  \
	((s)->len >= (s)->cap ? slice_grow((arena), (s), sizeof(*(s)->data)), \
			(s)->data + (s)->len++ : (s)->data + (s)->len++)

// Grows the specified slice by the given size.
void slice_grow(Arena* mem, void* slice, const size_t size);

#endif // SLICE_H
