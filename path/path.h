#ifndef PATH_H
#define PATH_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct Arena Arena;

// path_append appends the path b to the end of a.
char* path_append(Arena* mem, char* a, char* b);

// path_parent returns the parent path of a, or a if a has no parent.
char* path_parent(Arena* mem, char* a);

#endif // PATH_H
