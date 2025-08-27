#include "frontmatter.h"
#include <ctype.h>

String FrontMatter_Find(const String text) {
	size_t start = text.len + 1;
	for (size_t i = 0; i < text.len; i++) {
		if (isspace(text.data[i])) continue;
		if (i + 3 < text.len && (strncmp(text.data + i, "---\n", 4) == 0 || strncmp(text.data + i, "---\r", 4) == 0)) {
			start = i;
		}
		break;
	}
	if (start > text.len) {
		return STRNIL;
	}

	size_t end = text.len + 1;
	int isInSingleQuotes = 0;
	int isInDoubleQuotes = 0;
	for (size_t i = start + 3; i < text.len; i++) {
		if (text.data[i] == '"') isInDoubleQuotes = !isInDoubleQuotes;
		if (text.data[i] == '\'') isInSingleQuotes = !isInSingleQuotes;
		if (i + 2 < text.len && strncmp(text.data + i, "---", 3) == 0 && !isInDoubleQuotes && !isInSingleQuotes) {
			end = i + 2;
			break;
		}
	}
	if (end > text.len) {
		return STRNIL;
	}

	return (String){ text.data + start, end - start + 1 };
}

void FrontMatter_Process(const String fm, KVDict* kvd) {
	for (size_t i = 0; i < fm.len; i++) {
		if (isspace(fm.data[i])) {
			continue;
		} else {
			size_t keyStart = i, keyEnd = i;

			size_t j = i;
			while (j < fm.len && isspace(fm.data[j]) && fm.data[j] != '\n' && fm.data[j] != '\r') {
				j++;
			}
			keyStart = j;
			while (j < fm.len && !isspace(fm.data[j])) {
				j++;
			}
			keyEnd = j - 1;

			size_t valStart = keyEnd, valEnd = keyEnd;
			while (j < fm.len && isspace(fm.data[j]) && fm.data[j] != '\n' && fm.data[j] != '\r') {
				j++;
			}
			valStart = j;
			while (j < fm.len && fm.data[j] != '\n' && fm.data[j] != '\r') {
				j++;
			}
			valEnd = j - 1;

			String key = (String){ fm.data + keyStart, keyEnd - keyStart + 1 };
			String val = (String){ fm.data + valStart, valEnd - valStart + 1 };
			kvd_append(kvd, key, valEnd < valStart ? STRNIL : val);

			i = j;
		}
	}
}
