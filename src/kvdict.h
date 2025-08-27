#ifndef JELLY_KVDICT_H_INCLUDED_
#define JELLY_KVDICT_H_INCLUDED_
#pragma once

#include <stddef.h>
#include "str.h"

typedef struct {
	String key, value;
} KeyValue;

typedef struct {
	KeyValue* kv;
	size_t amt, cap;
} KVDict;

KVDict kvd_init(void);
void kvd_append(KVDict* kvd, const String key, const String value);
void kvd_destroy(KVDict* kvd);

// Returns the position of the first occurance of any key inside the `kvd`.
// *foundIndex is set to the index at which the `KeyValue` occurs in `kvd`.
long long kvd_findsubstr(const String s1, const KVDict* kvd, unsigned* foundIndex);

#endif // JELLY_KVDICT_H_INCLUDED_
