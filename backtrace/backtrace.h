#ifndef BACKTRACE_H
#define BACKTRACE_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena Arena;

#define BACKTRACE_TRUNCATED (1)

typedef struct Backtrace {
	int64_t status;
	size_t  len;
	union {
		char*      error_reason;
		uintptr_t* addresses;
	};

	bool   skip_specified;
	bool   max_specified;
	size_t skip;
	size_t max;
	Arena* mem;
} Backtrace;

// backtrace captures a backtrace of the current program, and returns a list of
// addresses in the backtrace. If max_addresses is greater than 0, then we
// return at most
//
// Everything will be allocated in the provided arena.
//
// Debug information is not required for this, but it requires the unwind API
// defined at <https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html>.
//
// On success, len is the number of addresses returned, and addresses points to
// a newly allocated array of addresses, starting at the function which called
// backtrace.
//
// If there was an error, the status member of the return value will be set to
// some negative value, and error_reason may be set to a string describing why
// the error occurred (len will be the length of this string not including the
// NUL terminator), depending on which error occurred.
//
// The following are the possible error codes:
//
// [BACKTRACE_TRUNCATED]
//      Everything succeeded, but max_addresses caused us to stop before the
//      end of the trace.
//
Backtrace backtrace(Arena* mem, size_t max_addresses, size_t skip);

#ifdef __cplusplus
}
#endif

#endif // BACKTRACE_H
