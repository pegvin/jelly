#ifdef TARGET_LINUX

#include "os/os.h"

#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>

NORETURN void os_abort(S32 exit_code) {
	exit(exit_code);
}

void* os_memory_reserve(U64 size) {
	return mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

void os_memory_unreserve(void* memory, U64 size) {
	munmap(memory, size);
}

void os_memory_commit(void* memory, U64 size) {
	mprotect(memory, size, PROT_READ | PROT_WRITE);
}

void os_memory_uncommit(void* memory, U64 size) {
	mprotect(memory, size, PROT_NONE);
}

#endif // TARGET_LINUX
