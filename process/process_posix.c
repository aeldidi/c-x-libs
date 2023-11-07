#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <spawn.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <unistd.h>

#include "c.eldidi.org/x/arena"
#include "c.eldidi.org/x/containers"
#include "c.eldidi.org/x/fs"

void
process_exec(Arena* mem, Arena scratch, int argc, char* argv[])
{
	char** exec_argv = arena_make(mem, char*, argc + 1);
	for (int i = 0; i < argc; i += 1) {
		exec_argv[i] = argv[i];
	}

	char* program = fs_resolve(mem, scratch, exec_argv[0]);
	if (program == NULL) {
		return;
	}

	exec_argv[0] = program;
	execv(exec_argv[0], exec_argv);
}

extern char** environ;

// Spawns a new process and returns a handle to the process.
Process
process_spawn(Arena scratch, int argc, char* argv[])
{
	Process result = {};

	int pipefd[2];
	if (pipe(pipefd) < 0 || fcntl(pipefd[0], F_SETFD, O_CLOEXEC) < 0 ||
			fcntl(pipefd[1], F_SETFD, O_NONBLOCK | O_CLOEXEC) <
					0) {
		result.status = PROCESS_SPAWN_FAILURE;
		return result;
	}

	pid_t ret = fork();
	if (ret < 0) {
		result.status = PROCESS_SPAWN_FAILURE;
		return result;
	}

	if (ret == 0) {
		// child
		close(pipefd[0]);

		process_exec(&scratch, scratch, argc, argv);
		uint8_t a = 0;
		write(pipefd[1], &a, sizeof(uint8_t));
		exit(EXIT_FAILURE);
	}

	close(pipefd[1]);
	uint8_t a = 0;
	// if this succeeds, the exec must have failed.
	if (read(pipefd[0], &a, sizeof(uint8_t)) == sizeof(uint8_t)) {
		result.status = PROCESS_SPAWN_FAILURE;
		return result;
	}

	result.native_handle = (uintptr_t)ret;
	return result;
}

// Returns true if the process has finished executing. If block is true, then
// blocks and waits until the process is done.
bool
process_wait(Process p, bool block)
{
retry:;
	int flags = 0;
	if (block) {
		flags |= WNOHANG;
	}

	int ret = waitpid((pid_t)p.native_handle, NULL, flags);
	if (ret < 0) {
		if (errno == EINTR) {
			goto retry;
		}
		return true;
	}

	return ret != 0;
}
