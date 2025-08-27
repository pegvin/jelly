#ifndef JELLY_FRONTMATTER_H_INCLUDED_
#define JELLY_FRONTMATTER_H_INCLUDED_
#pragma once

#include "str.h"
#include "kvdict.h"

/*
	FrontMatter is expected in the form:
		- Whitespace is defined as: en.cppreference.com/w/c/string/byte/isspace
		- Can be only preceded by whitespace
		- Three dashes (---) mark the start & end of FrontMatter in a text
		- Content is Key Value pairs
			- Key & Value are separated by whitespace
			- Key starts from first non-whitespace character & ends with whitespace character(s)
			- Value starts from first non-whitespace character AFTER key & ends with newline
			- Newline can either be \r or \n
		- Valid FrontMatter Example:
			```
			---
			title Hello World!
			id 236
			date 26 Sep, 2024
			---
			```
			Where `title`, `id` & `date` are keys & `Hello World!`, `236` & `26 Sep, 2024` are values respectively
*/
String FrontMatter_Find(const String text);
void FrontMatter_Process(const String fm, KVDict* kvd);

#endif // JELLY_FRONTMATTER_H_INCLUDED_
