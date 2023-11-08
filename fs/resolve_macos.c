#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/str"

char*
fs_resolve(Arena* mem, Arena scratch, char* name)
{
	assert(mem != NULL);
	assert(name != NULL);

	// I give up. use realpath and copy the malloc'd buffer into the arena.
	char* result_malloc = realpath(name, NULL);
	if (result_malloc == NULL) {
		return NULL;
	}

	size_t len    = strlen(result_malloc);
	char*  result = arena_make(mem, char, len + 1);
	memcpy(result, result_malloc, len);
	free(result_malloc);

	return result;
}
