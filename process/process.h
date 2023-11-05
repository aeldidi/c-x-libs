#ifndef PROCESS_H
#define PROCESS_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROCESS_SPAWN_FAILURE (-1)

typedef struct Arena Arena;

// process_exec is a cross platform version of UNIX exec(). Only returns if it
// fails.
void process_exec(Arena* mem, Arena scratch, int argc, char* argv[]);

typedef struct Process {
	int64_t   status;
	uintptr_t native_handle;
} Process;

// Spawns a new process and returns a handle to the process.
Process process_spawn(Arena scratch, int argc, char* argv[]);

// Returns true if the process has finished executing. If block is true, then
// blocks and waits until the process is done.
bool process_wait(Process p, bool block);

#ifdef __cplusplus
}
#endif

#endif // PROCESS_H
