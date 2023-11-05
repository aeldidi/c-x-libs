#ifndef SLICE_H
#define SLICE_H
#include <inttypes.h>
#include <stddef.h>

#include "c.eldidi.org/x/arena"

#define slice_push(s, arena)                                                  \
	((s)->len >= (s)->cap ? grow(s, sizeof(*(s)->data), arena),           \
			(s)->data + (s)->len++ : (s)->data + (s)->len++)

// Grows the specified slice by the given size.
void slice_grow(Arena* mem, void* slice, const size_t size);

#endif // SLICE_H
