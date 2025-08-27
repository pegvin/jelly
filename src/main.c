#include "fs.h"
#include "str.h"
#include "kvdict.h"
#include "frontmatter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cmark.h>
#include <strings.h>

#define ARENA_IMPLEMENTATION
#include "arena.h"

String ProcessMarkdown(arena_t* arena, const String markdown) {
	String rendered = STRNIL;
	cmark_node* doc = cmark_parse_document(markdown.data, markdown.len, CMARK_OPT_DEFAULT);
	if (doc != NULL) {
		rendered.data = cmark_render_html(doc, CMARK_OPT_DEFAULT | CMARK_OPT_UNSAFE);
		if (rendered.data != NULL) {
			rendered.len = strlen(rendered.data);
			char* new = arena_strdup(arena, rendered.data, rendered.len + 1);
			free(rendered.data);
			rendered.data = new;
		}

		cmark_node_free(doc);
	}
	return rendered;
}

void RenderFile(arena_t* arena, const String inputFilePath, const String outputFilePath, const String template) {
	KVDict kvd = kvd_init();
	String markdown = fs_read_file(arena, inputFilePath.data), restContent = markdown;
	String frontMatter = FrontMatter_Find(markdown);
	if (!str_isnil(frontMatter)) {
		restContent.data = frontMatter.data + frontMatter.len;
		restContent.len = markdown.len - (frontMatter.data - markdown.data + 1) - frontMatter.len;
		FrontMatter_Process(frontMatter, &kvd);
	}

	// Transform Key `key` to `{{key}}`
	for (size_t i = 0; i < kvd.amt; i++) {
		KeyValue* kv = &kvd.kv[i];
		kv->key.len += 4;
		kv->key.data = realloc(kv->key.data, kv->key.len + 1);
		for (size_t j = kv->key.len - 2 - 1; j >= 2; j--) {
			kv->key.data[j] = kv->key.data[j - 2];
		}
		kv->key.data[0] = kv->key.data[1] = '{';
		kv->key.data[kv->key.len - 1] = kv->key.data[kv->key.len - 2] = '}';
		kv->key.data[kv->key.len] = '\0';
	}

	kvd_append(&kvd, STRLIT("{{content}}"), ProcessMarkdown(arena, restContent));

	String outputText = str_duplicate(template);

	long long processed = 0;
	while (1) {
		unsigned foundIndex = 0;
		long long occurance = kvd_findsubstr(
			(String){ outputText.data + processed, outputText.len - processed },
			&kvd, &foundIndex
		);
		if (occurance < 0) break;

		String new = str_replace(
			outputText, kvd.kv[foundIndex].value,
			occurance + processed, kvd.kv[foundIndex].key.len
		);
		if (!str_isnil(new)) {
			free(outputText.data);
			outputText = new;
		}
		processed += occurance + kvd.kv[foundIndex].value.len;
	}

	if (fs_write_file(outputFilePath, outputText.data, outputText.len)) {
		printf("%s -> %s (Failed)\n", inputFilePath.data, outputFilePath.data);
	} else {
		printf("%s -> %s\n", inputFilePath.data, outputFilePath.data);
	}

	if (!str_isnil(outputText)) free(outputText.data);
	kvd_destroy(&kvd);
}

void RenderDir(const String root, const String build, const String template) {
	if (!fs_is_dir(build.data) && fs_make_dir(build.data)) {
		printf("Failed to make '%s'\n", build.data);
		exit(1);
	}

	String name = STRNIL;
	int isDir = 0;
	int failed = 0;
	dir_t dir = fs_list_dir_start(root.data);

	arena_t arena;
	arena_make(&arena);

	while (!(failed = fs_list_dir(dir, &name, &isDir)) && name.data) {
		if (isDir) {
			String newRoot = fs_join_path(&arena, root, name);
			String newBuild = fs_join_path(&arena, build, name);
			RenderDir(newRoot, newBuild, template);
		} else {
			int extIndex = fs_get_extension(name);
			if (
				extIndex < 0 ||
				(strncasecmp(".md", name.data + extIndex, 4) != 0 &&
				strncasecmp(".markdown", name.data + extIndex, 10) != 0)
			) continue;

			String inputFilePath = fs_join_path(&arena, root,  name);
			String outputFilePath = STRNIL;
			String outputFileName = str_replace(name, STRLIT("html"), extIndex + 1, name.len - extIndex - 1);
			if (!str_isnil(outputFileName)) {
				outputFilePath = fs_join_path(&arena, build, outputFileName);
				free(outputFileName.data);
			}

			RenderFile(&arena, inputFilePath, outputFilePath, template);
		}
	}

	arena_destroy(&arena);

	if (failed) {
		printf("Failed to list contents of '%s'\n", root.data);
		exit(1);
	}
}

void PrintUsage(void) {
	printf(
		"Usage: jelly [options]\n"
		"[options]:\n"
		"  -i Input directory [Default: www]\n"
		"  -o Output directory [Default: www-build]\n"
		"  -t Template file [Default: Input directory/template.html]\n"
		"  -v Print version\n"
		"  -h Print help message\n"
	);
}

int main(int argc, char** argv) {
	String root = STRLIT("www");
	String build = STRLIT("www-build");
	String templatePath = STRNIL;

	for (int i = 1; i < argc; i++) {
		if (strncmp("-i", argv[i], 3) == 0) {
			if (i + 1 < argc) {
				root = StrInit(argv[i + 1]);
				i++;
			} else {
				printf("Invalid usage!\n");
				PrintUsage();
				return 1;
			}
		} else if (strncmp("-o", argv[i], 3) == 0) {
			if (i + 1 < argc) {
				build = StrInit(argv[i + 1]);
				i++;
			} else {
				printf("Invalid usage!\n");
				PrintUsage();
				return 1;
			}
		} else if (strncmp("-t", argv[i], 3) == 0) {
			if (i + 1 < argc) {
				templatePath = StrInit(argv[i + 1]);
				i++;
			} else {
				printf("Invalid usage!\n");
				PrintUsage();
				return 1;
			}
		} else if (strncmp("-v", argv[i], 3) == 0) {
			printf("Jelly v0.1.1\n");
			return 0;
		} else if (strncmp("-h", argv[i], 3) == 0) {
			PrintUsage();
			return 0;
		} else {
			printf("Invalid usage!\n");
			PrintUsage();
			return 1;
		}
	}

	arena_t arena;
	arena_make(&arena);

	if (str_isnil(templatePath)) {
		templatePath = fs_join_path(&arena, root, STRLIT("template.html"));
	}
	if (str_isnil(templatePath) || !fs_is_file(templatePath.data)) {
		printf("Failed to load template!\n");
		arena_destroy(&arena);
		return 1;
	}
	String template = fs_read_file(&arena, templatePath.data);

	printf("Root: %s\n", root.data);
	printf("Build: %s\n", build.data);
	printf("Template: %s\n", templatePath.data);
	printf("CMark Version: %s\n", cmark_version_string());

	RenderDir(root, build, template);
	arena_destroy(&arena);

	return 0;
}
