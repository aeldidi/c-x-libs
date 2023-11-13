#ifndef FS_H
#define FS_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "c.eldidi.org/x/arena"

#ifdef __cplusplus
extern "C" {
#endif

#define FS_TYPE_FILE (0)
#define FS_TYPE_DIR  (1)
#define FS_TYPE_LINK (2)

typedef struct FileMetadata {
	// Either FS_TYPE_FILE, FS_TYPE_DIR, or FS_TYPE_LINK.
	int file_type;
	// The size of the file in bytes.
	size_t size;
	bool   readonly;
} FileMetadata;

typedef struct {
	int          status;
	FileMetadata result;
} StatResult;

// Returns the metadata for the filesystem entry specified by path.
//
// Currently, this is stat() on UNIX systems, and GetFileInformationByHandle()
// on Windows.
//
// In the return value, status is 0 on success, and a negative error code on
// failure.
StatResult fs_metadata(char* path);

// fs_foreach runs the given function for each file in the given directory.
// This function can return true to continue iteration, or false to stop
// iteration (equivalent to "continue" and "break").
//
// The parameter arg will be passed as the second parameter to your function.
//
// Returns false if the directory couldn't be opened for some reason.
bool fs_foreach_file(char* dirname, bool (*fn)(char*, void*), void* arg);

// fs_foreach_dir does the same thing as fs_foreach_file, but for directories.
// The folders "." and ".." are skipped on POSIX-like operating systems.
bool fs_foreach_dir(char* dirname, bool (*fn)(char*, void*), void* arg);

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
char* fs_resolve(Arena* mem, Arena scratch, char* name);

#ifdef __cplusplus
}
#endif

#endif // FS_H
