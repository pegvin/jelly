#include "base/arena.h"
#include "base/os/os.h"
#include <stdio.h>

int main(void) {
	Arena a = arena_init();

	PathArray arr = os_path_list_dir(&a, str_lit("./"));
	printf("Found %ld entries inside ./\n", arr.count);

	for EachIndex(i, arr.count) {
		printf(" - '%.*s'\n", (U32)arr.paths[i].size, arr.paths[i].data);
	}

	arena_release(&a);
	return 0;
}
