#ifndef SRC_BASE_STRING_H_
#define SRC_BASE_STRING_H_ 1
#pragma once

#include "base/types.h"

typedef struct {
	U8* data;
	U64 size;
} String;

#define str(d, s) ((String){ .data = (d), .size = (s) })
#define str_lit(s) str((U8*)(s), sizeof(s) - 1)
#define str_nil() str(NULL, 0)
#define str_is_nil(s) ((s).size == 0 || (s).data == NULL)

#endif // SRC_BASE_STRING_H_
