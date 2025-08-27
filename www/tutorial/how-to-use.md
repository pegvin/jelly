---
title How to use Jelly?
---

_Spoiler: It's very simple_

## Directory Structure

By default, Jelly expects your markdown source to be in `www` directory
and writes the output to `www-build` directory in cwd.  Use `-h` option
to learn more.

The output directory will contain the rendered HTML files in the
same structure they occur in the source directory, and the `.md`
extension will be replaced with `.html`.

It is important to note that only `.md` & `.markdown` (case-insensitive)
are considered to be markdown files.

Example: `www/index.md` turns to `www-build/index.html`.

## Templating

By default, `template.html` inside input directory will be loaded as
template and Jelly will look for known variables to substitute them
with their values.  Use `-h` option to learn more.

Variables take form of `{{var_name}}`, where `{{` & `}}` mark start and end
of the variable name and between them resides the variable name itself.

It is important to note that `{{var_name}}` is **NOT** equal to `{{ var_name }}`
or `{ {var_name} }`, vice-versa.

The `{{content}}` is the only in-built variable that holds the rendered HTML for
a markdown file, but other variable can be specified in the FrontMatter.

## FrontMatter

3-Dashes (`---`) mark the start & end of a source file's FrontMatter and the
FrontMatter can only be preceded by [whitespace](https://en.cppreference.com/w/c/string/byte/isspace)
and everything after the FrontMatter will be considered as the markdown.

Content inside the FrontMatter takes form of Key-Value pairs which are separated
by whitespace.

- The Key starts from first non-whitespace character & ends with whitespace character(s).
- The Value starts from first non-whitespace character AFTER key & ends with newline.
- The Value is optional
- Newline can either be `\r` or `\n`.

### Example Source

```
---
title Hello World!
date 26 Sep, 2024
---

This is my first post!
```

### Example Template

```
<!DOCTYPE html>
<html lang="en">
<head>
	<title>{{title}} - My Blog</title>
</head>
<body>
	<main>
		<header>
			<h1>{{title}}</h1>
			<time>{{date}}</time>
		</header>
		<article>{{content}}</article>
	</main>
</body>
</html>
```
