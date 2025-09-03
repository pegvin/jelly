#include <string.h>

#include "base/string.h"

String str_from_cstr(Arena* a, char* s, U64 len) {
	String ret = {0};
	ret.data = arena_alloc(a, len);
	ret.size = len;
	memcpy(ret.data, s, len);
	return ret;
}

char* cstr_from_str(Arena* a, String s) {
	U64 len = s.size + 1;
	char* str = arena_alloc(a, len);

	if (s.size > 0) {
		memcpy(str, s.data, s.size);
	}

	str[len - 1] = '\0';

	return str;
}
