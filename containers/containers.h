#ifndef MAP_H
#define MAP_H
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/hash"

// ======== Slice ======== //

// A slice is any struct which has a len, cap, and data member laid out like
// so:
typedef struct Slice {
	void*  data;
	size_t len;
	size_t cap;
} Slice;

#define slice_push(arena, s)                                                  \
	((s)->len >= (s)->cap ? slice_grow((arena), (s), sizeof(*(s)->data)), \
			(s)->data + (s)->len++ : (s)->data + (s)->len++)

// Grows the specified slice by the given size.
void slice_grow(Arena* mem, void* slice, const size_t size);

// ======== Hash Map ======= //

// In your structure, these can be any type so long as the types are laid out
// as in the Map structure.
#define map_keytype char*
#define map_valtype int

// A Map is any struct which has the following members.
typedef struct Map Map;
struct Map {
	Map*        child[4];
	map_keytype key;
	map_valtype value;
};

// The hash function and equals functions should look something like this.
typedef uint64_t map_hash_function(map_keytype);
typedef bool     map_equals_function(map_keytype, map_keytype);

// Here is an example of a map_get_function. Here we use fnv_1a_str as the hash
// function, since we use char* as the key type and strcmp as the equals
// function. Feel free to copy and paste this, and modify it for your own
// types as needed.
map_valtype*
map_get_function(Arena* arena, Map** map, map_keytype k)
{
	assert(map != NULL);
	for (uint64_t h = fnv_1a_str(k); *map; h <<= 2) {
		if ((*map)->key != NULL && strcmp(k, (*map)->key) == 0) {
			return &(*map)->value;
		}
		map = &(*map)->child[h >> 62];
	}

	if (!arena) {
		return NULL;
	}

	*map        = arena_make(arena, Map);
	(*map)->key = k;
	return &(*map)->value;
}

// Assuming your function has a signature similar to map_get_function, you can
// use this as a convenience macro similarly to slice_push.
//
// This macro assumes the function to get from type Map is Map_get.
#define map_get(arena, map_type, map, key) map_type##_get(arena, map, key)

#endif // MAP_H
