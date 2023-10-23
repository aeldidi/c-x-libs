#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "c.eldidi.org/c/arena"

void
process_exec(Arena* temp, int argc, char* argv[])
{
	char** exec_argv = arena_make(temp, char*, argc + 1);
	for (int i = 0; i < argc; i += 1) {
		exec_argv[i] = argv[i];
	}

	char* program = fs_resolve(temp, exec_argv[0]);
	if (program == NULL) {
		return;
	}

	exec_argv[0] = program;
	execv(exec_argv[0], exec_argv);
}
