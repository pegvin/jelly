#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

String str_duplicate(const String s) {
	if (str_isnil(s)) {
		return STRNIL;
	}

	String ret;
	ret.len = s.len;
	ret.data = malloc(ret.len + 1);
	memcpy(ret.data, s.data, s.len);
	ret.data[ret.len] = '\0';
	return ret;
}

// In `s1`, Replace `count` bytes starting at `start` with `s2`
String str_replace(const String s1, const String s2, unsigned start, unsigned count) {
	String ret = STRNIL;
	ret.len = s1.len + s2.len - count;
	ret.data = calloc(ret.len + 1, 1);
	if (ret.data == NULL) {
		printf("FATAL: Failed to allocated memory\n");
		exit(1);
	}

	memcpy(ret.data, s1.data, start); // Copy First part of s1
	memcpy(ret.data + start + s2.len, s1.data + start + count, s1.len - start - count); // Copy Second part of s1

	// NOTE: This is UB if s2 is STRNIL but it will most likely
	// work fine because s2.len will be zero so hopefully the
	// implementation doesn't try to access anything.
	memcpy(ret.data + start, s2.data, s2.len); // Copy s2

	return ret;
}
