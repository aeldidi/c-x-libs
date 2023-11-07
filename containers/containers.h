#ifndef MAP_H
#define MAP_H
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
#define map_keytype int
#define map_valtype int

// A Map is any struct which has the following members laid out like so:
// If your keytype is char*, you can omit the hash and equals function, and use
// map_get instead of hashmap_get.
typedef struct Map Map;
struct Map {
	Map*        child[4];
	map_keytype key;
	map_valtype value;
	uint64_t (*hash)(map_keytype);
	bool (*equals)(map_keytype, map_keytype);
};

#define hashmap_get(arena, map, key)                                          \
	({                                                                    \
		for (uint64_t h = map->hash(key); *map; h <<= 2) {            \
			if (map->equals(key, (*m)->key)) {                    \
				return &(*m)->value;                          \
			}                                                     \
			map = &(*map)->child[h >> 62];                        \
		}                                                             \
		if (!perm) {                                                  \
			return 0;                                             \
		}                                                             \
		*map        = arena_make(arena, map);                         \
		(*map)->key = key;                                            \
		&(*map)->value;                                               \
	})

#define map_get(arena, map, key)                                              \
	({                                                                    \
		for (uint64_t h = fnv_1a_str(key); *map; h <<= 2) {           \
			if (strcmp(key, (*map)->key)) {                       \
				return &(*map)->value;                        \
			}                                                     \
			map = &(*map)->child[h >> 62];                        \
		}                                                             \
		if (!perm) {                                                  \
			return 0;                                             \
		}                                                             \
		*map        = arena_make(arena, map);                         \
		(*map)->key = key;                                            \
		&(*map)->value;                                               \
	})

#endif // MAP_H
