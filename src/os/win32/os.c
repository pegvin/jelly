#ifdef TARGET_WINDOWS

#include "os/os.h"

#include <windows.h>

NORETURN void os_abort(S32 exit_code) {
	ExitProcess(exit_code);
}

void* os_memory_reserve(U64 size) {
	return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}

void os_memory_unreserve(void* memory, U64 size) {
	VirtualFree(memory, 0, MEM_RELEASE);
}

void os_memory_commit(void* memory, U64 size) {
	if (VirtualAlloc(memory, size, MEM_COMMIT, PAGE_READWRITE) == NULL) {
		os_abort_with_message(1, str8_lit("Failed to commit memory!"));
	}
}

void os_memory_uncommit(void* memory, U64 size) {
	VirtualFree(memory, size, MEM_DECOMMIT);
}
	
#endif // TARGET_WINDOWS
