#ifndef JELLY_STR_H_INCLUDED_
#define JELLY_STR_H_INCLUDED_
#pragma once

#include <stddef.h>
#include <string.h>

typedef struct {
	char* data; // With NULL for backwards compatibility with functions that still use it
	size_t len; // Without NULL
} String;

String str_duplicate(const String s);
String str_replace(const String s1, const String s2, unsigned start, unsigned count);

static inline int str_isnil(const String s) {
	return s.data == NULL || s.len == 0;
}

static inline String StrInit(char* str) {
	return (String){ str, strlen(str) };
}

// Initialize a `String` struct with a string literal
#define STRLIT(str) (String){ ""str"", sizeof(str) - 1 }
#define STRNIL (String){0, 0}

#endif // JELLY_STR_H_INCLUDED_
