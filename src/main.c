#include "base/arena.h"
#include "base/os/os.h"
#include <stdio.h>

static int _main(int argc, char *argv[]) {
	Arena a = arena_init();

	PathArray arr = os_path_list_dir(&a, str_lit("./"));
	printf("Found %lu entries inside ./\n", arr.count);

	for EachIndex(i, arr.count) {
		printf(" - '%.*s'\n", (U32)arr.paths[i].size, arr.paths[i].data);
	}

	arena_release(&a);
	return 0;
}

// Use UTF-8 Encoding on Windows instead of Windows codepage.
#ifdef TARGET_WINDOWS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int wmain(int argc, wchar_t *argv[]) {
	setlocale(LC_ALL, ".utf8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	char **mbargv;
	mbargv = (char**) malloc(argc * sizeof(char*));
	if (!mbargv) {
		puts("Out of memory");
		exit(3);
	}

	for (int i = 0; i < argc; i++) {
		size_t n;
		wcstombs_s(&n, NULL, 0, argv[i], 0);

		mbargv[i] = (char*) malloc(n + 1);
		if (!mbargv[i]) {
			puts("Out of memory");
			exit(3);
		}

		wcstombs_s(NULL, mbargv[i], n + 1, argv[i], n);
	}

	int errorcode = _main(argc, mbargv);

	for (int i = 0; i < argc; i++) {
		free(mbargv[i]);
	}

	free(mbargv);
	return errorcode;
}
#else
int main(int argc, char *argv[]) {
	return _main(argc, argv);
}
#endif
