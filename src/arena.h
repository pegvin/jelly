#ifndef JELLY_ARENA_H_INCLUDED_
#define JELLY_ARENA_H_INCLUDED_

#include <stdlib.h>

#define ARENA_BLOCK_SIZE 512
#define ARENA_ALLOC(size) malloc(size)
#define ARENA_REALLOC(oldptr, newsize) realloc(oldptr, newsize)
#define ARENA_FREE(ptr) free(ptr)

typedef struct {
	unsigned int size, pos;
	char *data;
} arena_block_t;

typedef struct {
	unsigned int numBlocks;
	arena_block_t* blocks;
} arena_t;

int arena_make(arena_t* arena); // Returns non-negative value on error
void* arena_alloc(arena_t* arena, unsigned int size); // Returns NULL on error
void* arena_calloc(arena_t* arena, unsigned int size); // Returns NULL on error
char* arena_strdup(arena_t* arena, const char* str, long long len_with_null_terminator); // Returns NULL on error
void arena_destroy(arena_t* arena);

#endif

#ifdef ARENA_IMPLEMENTATION

int arena_make(arena_t* out) {
	arena_t a;
	a.numBlocks = 1;
	a.blocks = (arena_block_t*)ARENA_ALLOC(sizeof(*a.blocks));
	if (a.blocks == NULL) return 1;

	a.blocks[0].pos = 0;
	a.blocks[0].size = ARENA_BLOCK_SIZE;
	a.blocks[0].data = (char*)ARENA_ALLOC(ARENA_BLOCK_SIZE);
	if (a.blocks[0].data == NULL) return 1;

	*out = a;

	return 0;
}

void* arena_alloc(arena_t* arena, unsigned int size) {
	int blockToUse = -1;
	for (unsigned int i = 0; i < arena->numBlocks; i++) {
		if (arena->blocks[i].size - (arena->blocks[i].pos) > size) {
			blockToUse = i;
			i++;
		}
	}

	if (blockToUse < 0) {
		arena->blocks = (arena_block_t*)ARENA_REALLOC(arena->blocks, (arena->numBlocks + 1) * sizeof(*arena->blocks));
		if (arena->blocks == NULL) {
			return NULL;
		}
		arena->numBlocks++;
		blockToUse = arena->numBlocks - 1;

		arena->blocks[blockToUse].pos = 0;
		arena->blocks[blockToUse].size = size;
		arena->blocks[blockToUse].data = (char*)ARENA_ALLOC(size);
		if (arena->blocks[blockToUse].data == NULL) {
			return NULL;
		}
	}

	void* ret = arena->blocks[blockToUse].data + arena->blocks[blockToUse].pos;
	arena->blocks[blockToUse].pos += size;

	return ret;
}

void* arena_calloc(arena_t* arena, unsigned int size) {
	void* data = arena_alloc(arena, size);
	if (data == NULL) return NULL;
	memset(data, 0, size);
	return data;
}

char* arena_strdup(arena_t* arena, const char* str, long long len_with_null_terminator) {
	char* new = arena_calloc(arena, len_with_null_terminator);
	if (new == NULL) return NULL;
	memcpy(new, str, len_with_null_terminator);
	return new;
}

void arena_destroy(arena_t* arena) {
	for (unsigned int i = 0; i < arena->numBlocks; i++) {
		ARENA_FREE(arena->blocks[i].data);
	}
	ARENA_FREE(arena->blocks);
	arena->numBlocks = 0;
	arena->blocks = NULL;
}

#endif // JELLY_ARENA_H_INCLUDED_
