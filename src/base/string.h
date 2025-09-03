#ifndef SRC_BASE_STRING_H_
#define SRC_BASE_STRING_H_ 1
#pragma once

#include "base/types.h"
#include "base/arena.h"

typedef struct {
	U8* data;
	U64 size;
} String;

#define str(d, s) ((String){ .data = (d), .size = (s) })
#define str_lit(s) str((U8*)(s), sizeof(s) - 1)
#define str_nil() str(NULL, 0)
#define str_is_nil(s) ((s).size == 0 || (s).data == NULL)

String str_from_cstr(Arena* a, char* s, U64 len);
char* cstr_from_str(Arena* a, String s);

#endif // SRC_BASE_STRING_H_
