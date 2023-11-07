#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "c.eldidi.org/x/arena"

char*
path_append(Arena* mem, char* a, char* b)
{
	assert(mem != NULL);
	assert(a != NULL);
	assert(b != NULL);
	size_t a_len  = strlen(a);
	size_t b_len  = strlen(b);
	size_t len    = a_len + b_len + 1;
	char*  result = arena_make(mem, char, len + 1);
	memcpy(result, a, a_len);
	result[a_len] = '/';
	memcpy(&result[a_len + 1], b, b_len);
	return result;
}

char*
path_parent(Arena* mem, char* a)
{
	assert(mem != NULL);
	assert(a != NULL);
	assert(false);
	// TODO
}
