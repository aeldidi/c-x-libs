#include <dlfcn.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/backtrace"

void
panic(Arena* mem, char* msg)
{
	fprintf(stderr, "panic: %s\n", msg);
	Backtrace b = backtrace(mem, 0, 1);
	if (b.status != 0) {
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "backtrace (most recent call last)\n");
	for (size_t i = 0; i < b.len; i += 1) {
		Dl_info di  = {};
		int     ret = dladdr((void*)b.data[i], &di);
		if (ret == 0) {
			fprintf(stderr, "%" PRIxPTR "in ???\n", b.data[i]);
			continue;
		}

		fprintf(stderr, "%s: %s + %" PRIxPTR "\n", di.dli_fname,
				di.dli_sname,
				b.data[i] - (uintptr_t)di.dli_saddr);
	}
	exit(EXIT_FAILURE);
}

int
main()
{
	char*   memory         = malloc(1 << 16);
	char*   memory_scratch = malloc(1 << 16);
	jmp_buf jb;
	if (memory == NULL || memory_scratch == NULL || setjmp(jb)) {
		fprintf(stderr, "memory allocation failure\n");
		return EXIT_FAILURE;
	}

	Arena mem = {
			.beg     = memory,
			.end     = memory + (1 << 16),
			.jmp_buf = jb,
	};
	Arena scratch = {
			.beg     = memory_scratch,
			.end     = memory_scratch + (1 << 16),
			.jmp_buf = jb,
	};

	panic(&mem, "poop");
}
