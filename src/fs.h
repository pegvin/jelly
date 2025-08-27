#ifndef JELLY_FS_H_INCLUDED_
#define JELLY_FS_H_INCLUDED_
#pragma once

#include "arena.h"
#include "str.h"

typedef void* dir_t;

/*
 Description:
  Opens a directory and returns a handle
 */
dir_t fs_list_dir_start(const char* dirPath);

/*
 Description:
  Loads `dir` contents one by one into `*name` with each
  subsequent function call, where `*isDir` is set to non-zero
  to indicate if the entry is a dir.
 Notes:
  - Memory pointed by `*name` is only valid till the next function call.
  - Returns non-zero value on error.
  - Sets `*name` to NULL to indicate end of contents.
 */
int fs_list_dir(dir_t dir, String* name, int* isDir);

/*
 Description:
  Call this to prematurely end `fs_list_dir` without
  having to wait for NULL entry.
 */
void fs_list_dir_abrupt_end(dir_t dir);

/*
 Description:
  Get `cc` part of `c:/aa/bb/cc` path
 Notes:
  - Returns index at which basename starts in the `path` variable
  - Returns negative value if not found
 */
size_t fs_get_basename(const String path);

/*
 Description:
  Get `c:/aa/bb/` part of `c:/aa/bb/cc` path
 Notes:
  - Returns non-negative index that points to
    END of the path of parent dir
  - Values are set to negative if not found
 */
int fs_get_parent_dir(const String path);

/*
 Description:
  Get `.txt` part of `c:/aa/bb/cc.txt` path
 Notes:
  - Returns index at which basename starts in the `path` variable
  - Returns negative value if not found
 */
int fs_get_extension(const String filePath);

/*
 Description:
  Returns non-zero value if given path is a file on filesystem
 */
int fs_is_file(const char* filePath);

/*
 Description:
  Returns non-zero value if given path is a directory on filesystem
 */
int fs_is_dir(const char* dirPath);

/*
 Description:
  Creates a directory at `path` path and returns non-zero value on error
 */
int fs_make_dir(const char* path);

/*
 Description:
  Allocates memory and reads all the bytes from `path` file
  and returns it, free-ing it is upto the caller.
 Notes:
  - Returns NULL on error
 */
String fs_read_file(arena_t* arena, const char* path);

/*
 Description:
  Writes `data` to file at `path` path
 Notes:
  - Returns non-zero value on error
 */
int fs_write_file(const String path, void* data, long long len);

/*
 Description:
  Returns file size of `filePath` file
 Notes:
  - Returns negative value on error
 */
long long fs_get_file_size(const char* filePath);

// Joins path `p1` & `p2` with appropriate path separator.
// Freeing the String is upto the caller.
// Calls exit(1) on Allocation Error.
String fs_join_path(arena_t* arena, String p1, String p2);

#endif // JELLY_FS_H_INCLUDED_
