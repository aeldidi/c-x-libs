#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fts.h>
#include <limits.h>

#include "c.eldidi.org/x/arena"

extern int errno;

bool
fs_foreach_file(char* dirname, bool (*fn)(char*, void*), void* arg)
{
	assert(dirname != NULL);
	assert(fn != NULL);

	char* paths[] = {dirname, NULL};
	FTS*  dir     = fts_open(
			     paths, FTS_PHYSICAL | FTS_NOCHDIR | FTS_NOSTAT, NULL);
	if (dir == NULL) {
		return false;
	}

	while (fts_read(dir) != NULL) {
		FTSENT* current = fts_children(dir, 0);
		for (; current != NULL; current = current->fts_link) {
			if ((current->fts_info & FTS_F) == 0) {
				continue;
			}

			fn(current->fts_name, arg);
		}
	}

	return fts_close(dir) == 0;
}

// realpath(3) has stupid arbitrary limits. We can do better.
#if defined(__linux__)
#include "./resolve_linux.c"
#elif defined(__APPLE__)
#include "./resolve_macos.c"
#else
#error "unknown OS"
#endif
