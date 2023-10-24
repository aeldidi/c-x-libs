#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <unwind.h>

#include "c.eldidi.org/x/arena"

static _Unwind_Reason_Code
unwind_callback(struct _Unwind_Context* ctx, void* arg)
{
	Backtrace* b  = arg;
	uintptr_t  ip = _Unwind_GetIP(ctx);

	// Skip as many addresses as we were told to.
	if (b->skip_specified && b->skip > 0) {
		b->skip -= 1;
		return _URC_NO_REASON;
	}

	if (b->max_specified && b->len == b->max) {
		b->status = BACKTRACE_TRUNCATED;
		return _URC_END_OF_STACK;
	}

	// bump up the arena. Since arenas allocate their memory contiguously,
	// this is like extending the array length.
	(void)arena_make(mem, uintptr_t);
	b->addresses[b->len] = ip;
	b->len += 1;

	return _URC_NO_REASON;
}

Backtrace
backtrace(Arena* mem, size_t max_addresses, size_t skip)
{
	Backtrace result = {
			.skip_specified = skip != 0,
			.skip           = skip,
			.max_specified  = max_addresses != 0,
			.max            = max_addresses,
			.addresses      = arena_make(mem, uintptr_t),
	};
	_Unwind_Backtrace(unwind_callback, &result);
	return result;
}
