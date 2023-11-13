#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/str"

#if !defined(O_PATH)
#define O_PATH 2097152
#endif

// This is implemented here despite the fact that it could be implemented just
// in terms of stat and whatnot because the /proc/self/fd trick can be used to
// simply retrieve the full path.
char*
fs_resolve(Arena* mem, Arena scratch, char* name)
{
	assert(mem != NULL);
	assert(name != NULL);

	size_t size = strlen(name);
	if (size < 1 || name[0] == '/') {
		return name;
	}

	int flags = O_NONBLOCK | O_CLOEXEC | O_PATH;
	int fd    = openat(AT_FDCWD, name, flags);
	if (fd < 0) {
		return NULL;
	}

	char*       path = str_format(&scratch, "/proc/self/fd/%d", fd);
	struct stat s    = {};
	if (lstat(path, &s) < 0) {
		return NULL;
	}

	size_t  len    = s.st_size + 1;
	char*   result = arena_make(mem, char, len);
	ssize_t nread  = readlink(path, result, len);
	if (nread < 0) {
		return NULL;
	}

	// stat doesn't include a NUL terminator (if one is present) in
	// st_size. This means we should never have nread = len.
	assert((size_t)nread != len);

	// If the file we want to resolve is a symlink to a another symlink
	// which links to the first one (a -> b and b -> a), this is an
	// infinite loop, and we return NULL in that case.
	struct stat a = {};
	fstat(fd, &a);
	struct stat b = {};
	if (stat(result, &b) < 0 || a.st_dev != b.st_dev ||
			a.st_ino != b.st_ino) {
		return NULL;
	}

	return result;
}
