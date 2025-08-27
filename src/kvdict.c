#include "kvdict.h"
#include <stdlib.h>
#include <stdio.h>

KVDict kvd_init(void) {
	KVDict s;
	s.amt = 0;
	s.cap = 100;
	s.kv = calloc(s.cap, sizeof(*s.kv));
	if (s.kv == NULL) {
		printf("FATAL: Failed to allocated memory\n");
		exit(1);
	}

	return s;
}

void kvd_append(KVDict* kvd, const String key, const String value) {
	for (size_t i = 0; i < kvd->amt; i++) {
		KeyValue* kv = &kvd->kv[i];
		if (key.len == kv->key.len && strncmp(key.data, kv->key.data, key.len) == 0) {
			String new = str_duplicate(value);
			if (kv->value.data) {
				free(kv->value.data);
			}
			kv->value = new;
			return;
		}
	}

	KeyValue kv = {0};
	kv.key = str_duplicate(key);
	kv.value = str_duplicate(value);

	if (kvd->cap < kvd->amt + 1) {
		size_t newCap = kvd->cap + 50;
		kvd->kv = realloc(kvd->kv, sizeof(*kvd->kv) * newCap);
		if (kvd->kv == NULL) {
			printf("FATAL: Failed to allocated memory\n");
			exit(1);
		}
		kvd->cap = newCap;
	}

	kvd->kv[kvd->amt++] = kv;
}

void kvd_destroy(KVDict* kvd) {
	for (size_t i = 0; i < kvd->amt; i++) {
		free(kvd->kv[i].key.data);
		free(kvd->kv[i].value.data);
	}
	free(kvd->kv);
	*kvd = (KVDict){0};
}

long long kvd_findsubstr(const String s1, const KVDict* kvd, unsigned* foundIndex) {
	for (size_t i = 0; i < s1.len; i++) {
		for (unsigned j = 0; j < kvd->amt && s1.len - i + 1 >= kvd->kv[j].key.len; j++) {
			if (strncmp(s1.data + i, kvd->kv[j].key.data, kvd->kv[j].key.len) == 0) {
				*foundIndex = j;
				return i;
			}
		}
	}

	return -1;
}
