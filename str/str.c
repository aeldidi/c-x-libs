#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/hash"
#include "github.com/aeldidi/encoding/utf8"

char*
str_format(Arena* mem, const char* format, ...)
{
	va_list ap;
	va_list bp;
	va_start(ap, format);
	va_copy(bp, ap);
	ptrdiff_t size   = vsnprintf(NULL, 0, format, ap);
	char*     result = arena_make(mem, char, size + 1);
	vsnprintf(result, size + 1, format, bp);
	va_end(ap);
	return result;
}

StrSlice
str_split(Arena* mem, const char* str, const uint32_t c)
{
	// Every string is beside each other in memory. If I call
	// split("a/b/c", '/') the strings will be "a\0b\0c\0" followed by an
	// array of pointers to them.
	StrSlice    result    = {};
	size_t      len       = strlen(str) + 1;
	const char* s         = str;
	size_t      s_len     = 0;
	size_t      ch_len    = 0;
	char*       first_str = NULL;
	size_t      num_strs  = 0;
	for (size_t i = 0; i < len; i += ch_len) {
		uint32_t ch = utf8_codepoint_decode(
				len - i, (uint8_t*)str + i, &ch_len);
		if (ch != c && ch != 0) {
			s_len += 1;
			continue;
		}

		char* to_append = arena_make(mem, char, s_len + 1);
		memcpy(to_append, s, s_len);
		if (first_str == NULL) {
			first_str = to_append;
		}
		s     = s + s_len + 1;
		s_len = 0;
		num_strs += 1;
	}

	result.len  = num_strs;
	result.data = arena_make(mem, char*, num_strs);
	size_t i    = 0;
	for (char* p = first_str; (void*)p < (void*)result.data;
			p += strlen(p) + 1, i += 1) {
		// printf("z: %zu\n", p - buf);
		result.data[i] = p;
	}

	return result;
}

uintptr_t
strpool_add(Arena* mem, Strpool* pool, const char* string)
{
	for (uint64_t h = fnv_1a_str(string); *m; h <<= 2) {
		if (equals(string, (*m)->key)) {
			return (uintptr_t)m;
		}
		m = &(*m)->child[h >> 62];
	}
	*m        = arena_make(mem, Strpool);
	(*m)->key = string;
	return (uintptr_t)m;
}

char*
strpool_get(Strpool* pool, const uintptr_t handle)
{
	if ((void*)handle == NULL) {
		return NULL;
	}

	return ((Strpool*)handle)->key;
}
