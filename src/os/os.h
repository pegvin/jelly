#ifndef SRC_OS_OS_H
#define SRC_OS_OS_H 1

#include "base/types.h"
#include "base/math.h"
#include "base/string.h"
#include "base/arena.h"

typedef struct {
	U64 value;
} OS_Handle;

typedef struct {
	String* items;
	U64     count;
} OS_PathArray;

// Type Functions
OS_Handle os_handle_zero(void);
bool      os_handle_match(OS_Handle a, OS_Handle b);
bool      os_handle_is_zero(OS_Handle a);

// Memory
void* os_memory_reserve(U64 size);
void  os_memory_unreserve(void* memory, U64 size);
void  os_memory_commit(void* memory, U64 size);
void  os_memory_uncommit(void* memory, U64 size);

// Abortion
NORETURN void os_abort(S32 exit_code);
NORETURN void os_abort_with_message(S32 exit_code, String message);

// Paths
typedef struct {
	String* paths;
	U64     count;
} PathArray;

PathArray os_path_list_dir(Arena* a, String path);
Rng1DU64  os_path_basename(String path);
Rng1DU64  os_path_extension(String path);

#endif // SRC_OS_OS_H
