#ifndef FS_H
#define FS_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct Arena Arena;

// fs_foreach runs the given function for each file in the given directory.
// This function can return true to continue iteration, or false to stop
// iteration (equivalent to "continue" and "break").
//
// The parameter arg will be passed as the second parameter to your function.
//
// Returns false if the directory couldn't be opened for some reason.
bool fs_foreach_file(char* dirname, bool (*fn)(char*, void*), void* arg);

// Given a path relative to the current path, fs_resolve resolves a it to a
// path relative to the root directory (/).
//
// On Windows, the drive letter is always included, so there is no ambiguity on
// what is considered "the root".
//
// If the path given begins with '/', it is simply returned.
//
// Returns NULL if it can't for some reason.
//
// The result will be allocated in mem.
char* fs_resolve(Arena* mem, char* name);

#endif // FS_H
