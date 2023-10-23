#ifndef STR_H
#define STR_H
#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

// str_format returns a newly allocated format string.
char* str_format(Arena* mem, char* format, ...);

#ifdef __cplusplus
}
#endif

#endif // STR_H
