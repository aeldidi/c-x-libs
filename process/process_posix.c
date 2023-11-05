#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/fs"

void
process_exec(Arena* mem, Arena scratch, int argc, char* argv[])
{
	char** exec_argv = arena_make(temp, char*, argc + 1);
	for (int i = 0; i < argc; i += 1) {
		exec_argv[i] = argv[i];
	}

	char* program = fs_resolve(mem, &scratch, exec_argv[0]);
	if (program == NULL) {
		return;
	}

	exec_argv[0] = program;
	execv(exec_argv[0], exec_argv);
}
