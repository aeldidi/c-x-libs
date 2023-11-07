#ifndef STR_H
#define STR_H
#include <inttypes.h>
#include <stddef.h>

#include "c.eldidi.org/x/containers"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

// str_format returns a newly allocated format string.
char* str_format(Arena* mem, const char* format, ...);

typedef struct StrSlice {
	char** data;
	size_t len;
	size_t cap;
} StrSlice;

// str_split splits the string by the codepoint c.
StrSlice str_split(Arena* mem, const char* str, const uint32_t c);

typedef struct Strpool Strpool;
struct Strpool {
	Strpool* child[4];
	char*    key;
};

/*
// strpool_add adds a string into an intern pool, returning a handle to the
// string.
uintptr_t strpool_add(Arena* mem, Strpool* pool, const char* string);

// strpool_get retrieves a string from the pool. If the handle is 0, returns
// NULL.
char* strpool_get(Strpool* pool, const uintptr_t handle);
*/

#ifdef __cplusplus
}
#endif

#endif // STR_H
