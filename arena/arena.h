#ifndef ARENA_H
#define ARENA_H
#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// If we have GCC or Clang, we can get some free optimization and ASAN runtime
// checking for free by defining this.
#if !defined(ALLOC_ATTIBUTES)
#if defined(__has_attribute)
#if __has_attribute(__malloc__)
#define ALLOC_ATTRIBUTES                                                      \
	__attribute((__malloc__, alloc_size(2, 4), alloc_align(3)))
#else
#define ALLOC_ATTRIBUTES
#endif // __has_attribute(__malloc__)
#else
#define ALLOC_ATTRIBUTES
#endif // defined(__has_attribute)
#endif // !defined(ALLOC_ATTRIBUTES)

#define ARENA_NOZERO (1 << 0) // disables initializing memory to 0

typedef struct Arena {
	char* beg;
	char* end;
	void* jmp_buf;
} Arena;

// arena_alloc allocates memory inside the given [[Arena]]. You can specify
// the size in bytes, alignment, and number of objects of the given size and
// alignment. Additionally, the allocation behaviour can be customized using
// the flags parameter.
//
// Currently, only ARENA_NOZERO is supported as a flag, which prevents the
// memory allocated from being initialized to zero before returning.
//
// On error, longjmp is used to jump to the jmp_buf contained in the [[Arena]].
ALLOC_ATTRIBUTES
void* arena_alloc(Arena* a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count,
		uint32_t flags);

// T* arena_make(Arena* a, type T, [ptrdiff_t count, uint32_t flags]) allocates
// a new T* in the arena. The count and flags parameters are optional and are
// the same as in arena_alloc.
#define arena_make(...)                                                       \
	arena_makex(__VA_ARGS__, arena_make4, arena_make3, arena_make2)(      \
			__VA_ARGS__)

// Don't use directly. Just a wrapper to allow selecting between each
// arena_makeN macro depending on the number of parameters passed to
// arena_make.
#define arena_makex(a, b, c, d, e, ...) e

#define arena_make2(a, t)    (t*)arena_alloc(a, sizeof(t), _Alignof(t), 1, 0)
#define arena_make3(a, t, n) (t*)arena_alloc(a, sizeof(t), _Alignof(t), n, 0)
#define arena_make4(a, t, n, f)                                               \
	(t*)arena_alloc(a, sizeof(t), _Alignof(t), n, f)

#ifdef __cplusplus
}
#endif

#endif // ARENA_H
