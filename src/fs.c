#define _DEFAULT_SOURCE
#include <stddef.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fs.h"

dir_t fs_list_dir_start(const char* dirPath) {
	DIR* dir = opendir(dirPath);
	return dir;
}

int fs_list_dir(dir_t dir, String* name, int* isDir) {
tryAgain:
	errno = 0;
	struct dirent* ent = readdir(dir);
	if (ent == NULL) {
		if (errno != 0) { // readdir returned NULL because error occurred
			closedir(dir);
			return 1;
		} else { // readdir returned NULL because no more entries
			*name = STRNIL;
			closedir(dir);
		}
	} else if (ent->d_name[0] == '.') {
		goto tryAgain; // skip "." & ".." entries
	} else {
		name->data = ent->d_name;
		name->len = strlen(name->data);
		*isDir = ent->d_type == DT_DIR;
	}

	return 0;
}

void fs_list_dir_abrupt_end(dir_t dir) {
	if (dir) {
		closedir(dir);
	}
}

int fs_make_dir(const char* path) {
	int res = mkdir(path, S_IRWXU);
	if (res != 0 && errno != EEXIST) {
		return 1;
	}
	return 0;
}

size_t fs_get_basename(const String path) {
	size_t index = 0;
	for (size_t i = 0; i < path.len; i++) {
		if (
			(path.data[i] == '/' || path.data[i] == '\\') &&
			i + 1 < path.len &&
			(path.data[i + 1] != '/' && path.data[i + 1] != '\\')
		) {
			index = i + 1;
		}
	}
	return index;
}

int fs_get_extension(const String filePath) {
	for (int i = filePath.len - 1; i >= 0; i--) {
		if (filePath.data[i] == '/' || filePath.data[i] == '\\') {
			return -1;
		}
		if (filePath.data[i] == '.') {
			return i;
		}
	}

	return -1;
}

int fs_get_parent_dir(const String path) {
	// using - 2 since it will filter out any trailing path separator
	for (int i = path.len - 2; i >= 0; i--) {
		if (path.data[i] == '/' || path.data[i] == '\\') {
			return i;
		}
	}

	return -1;
}

int fs_is_file(const char* filePath) {
	struct stat st;
	return stat(filePath, &st) >= 0 && S_ISREG(st.st_mode) == 1;
}

int fs_is_dir(const char* dirPath) {
	struct stat st;
	return stat(dirPath, &st) >= 0 && S_ISDIR(st.st_mode) == 1;
}

long long fs_get_file_size(const char* filePath) {
	FILE* f = fopen(filePath, "r");
	if (f == NULL) return -1;

	long long size = (fseek(f, 0, SEEK_END) >= 0) ? ftell(f) : -1;
	fclose(f);
	return size;
}

String fs_read_file(arena_t* arena, const char* path) {
	long long fSize = fs_get_file_size(path);
	if (fSize < 0) {
		return STRNIL;
	}

	FILE* f = fopen(path, "r");
	if (f == NULL) {
		return STRNIL;
	}

	arena_t temp;
	arena_make(&temp);
	char* text = arena_calloc(&temp, fSize + 1);
	if (text == NULL) {
		fclose(f);
		return STRNIL;
	}
	if ((long long)fread(text, 1, fSize, f) != fSize) {
		fclose(f);
		arena_destroy(&temp);
		return STRNIL;
	}
	fclose(f);

	String outText;
	outText.data = arena_calloc(arena, fSize + 1);
	outText.len = fSize;
	memcpy(outText.data, text, fSize + 1);

	arena_destroy(&temp);
	return outText;
}

int fs_write_file(const String path, void* data, long long len) {
	FILE* f = fopen(path.data, "w");
	if (f == NULL) {
		return 1;
	}

	if ((long long)fwrite(data, 1, len, f) != len) {
		fclose(f);
		return 1;
	}

	fclose(f);

	return 0;
}

String fs_join_path(arena_t* arena, String p1, String p2) {
	while (p2.data[0] && (p2.data[0] == '/' || p2.data[0] == '\\')) {
		p2.data++;
	}

	int hasTrailingSlash = p1.data[p1.len] == '/' || p1.data[p1.len] == '\\';

	long long finalSize = p1.len + p2.len;
	if (!hasTrailingSlash) finalSize++;

	String final = { arena_calloc(arena, finalSize + 1), finalSize };
	if (final.data == NULL) {
		printf("FATAL: Failed to allocated memory\n");
		exit(1);
	}

	memcpy(final.data, p1.data, p1.len);
	final.data[p1.len] = '/';
	memcpy(final.data + p1.len + 1, p2.data, p2.len);

	return final;
}
