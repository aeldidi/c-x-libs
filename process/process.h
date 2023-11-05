#ifndef PROCESS_H
#define PROCESS_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

// process_exec is a cross platform version of UNIX exec(). Only returns if it
// fails.
void process_exec(Arena* mem, Arena scratch, int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif // PROCESS_H
