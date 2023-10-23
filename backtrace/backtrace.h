#ifndef BACKTRACE_H
#define BACKTRACE_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

typedef struct Backtrace Backtrace;

// backtrace captures a backtrace of the current program, and returns a list of
// addresses in the backtrace.
//
// Debug information is not required for this.
int backtrace(Arena* mem);

#ifdef __cplusplus
}
#endif

#endif // BACKTRACE_H
