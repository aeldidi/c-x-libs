#include <inttypes.h>
#include <stddef.h>
#include <string.h>

#include "c.eldidi.org/x/arena"

// Grows the specified slice by the given size.
void
slice_grow(Arena* mem, void* slice, const size_t size)
{
	struct {
		void*  data;
		size_t len;
		size_t cap;
	} replica;
	memcpy(&replica, slice, sizeof(replica));

	if (replica.cap == 0) {
		replica.cap = 1;
	}
	ptrdiff_t align = 16;
	void*     data  = arena_alloc(mem, 2 * size, align, replica.cap, 0);
	replica.cap *= 2;
	if (replica.len != 0) {
		memcpy(data, replica.data, size * replica.len);
	}
	replica.data = data;

	memcpy(slice, &replica, sizeof(replica));
}
