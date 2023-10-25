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

StatResult
fs_metadata(char* path)
{
	StatResult  result = {};
	struct stat s;
	int         ret = stat(path, &s);
	if (ret < 0) {
		result.status = -1;
		return result;
	}

	if (S_ISREG(s.st_mode)) {
		result.result.file_type = FS_TYPE_FILE;
	} else if (S_ISDIR(s.st_mode)) {
		result.result.file_type = FS_TYPE_DIR;
	} else if (S_ISLNK(s.st_mode)) {
		result.result.file_type = FS_TYPE_LINK;
	} else {
		fprintf(stderr, "TODO: other file_types.\n");
		exit(EXIT_FAILURE);
	}

	result.result.size     = s.st_size;
	result.result.readonly = s.st_mode & (S_IRUSR | S_IXUSR);
	return result;
}

// realpath(3) has stupid arbitrary limits. We can do better.
#if defined(__linux__)
#include "./resolve_linux.c"
#elif defined(__APPLE__)
#include "./resolve_macos.c"
#else
#error "unknown OS"
#endif
