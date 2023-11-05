#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <unwind.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/slice"

static _Unwind_Reason_Code
unwind_callback(struct _Unwind_Context* ctx, void* arg)
{
	Backtrace* b  = arg;
	uintptr_t  ip = _Unwind_GetIP(ctx);
	if ((void*)ip == NULL) {
		return _URC_END_OF_STACK;
	}

	// Skip as many addresses as we were told to.
	if (b->skip_specified && b->skip > 0) {
		b->skip -= 1;
		return _URC_NO_REASON;
	}

	if (b->max_specified && b->len == b->max) {
		b->status = BACKTRACE_TRUNCATED;
		return _URC_END_OF_STACK;
	}

	*slice_push(b->mem, b) = ip;
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
			.mem            = mem,
	};
	_Unwind_Backtrace(unwind_callback, &result);
	return result;
}
