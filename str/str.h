#ifndef STR_H
#define STR_H
#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

// str_format returns a newly allocated format string.
char* str_format(Arena* mem, const char* format, ...);

typedef struct StrSlice {
	char** data;
	size_t len;
	size_t cap;
} StrSlice;

// str_split splits the string by the codepoint c.
StrSlice str_split(Arena* mem, const char* str, const uint32_t c);

#ifdef __cplusplus
}
#endif

#endif // STR_H
