---
title How to build from source
---

Before building from source, make sure you have the
following software installed:
- Git
- POSIX-Compliant Shell
- C99 Compiler (Default: gcc)
- CMark Library (Debian/Ubuntu: `libcmark-dev`)

```
git clone https://github.com/pegvin/jelly.git
cd jelly
./build.sh release
```
and you should have a binary at `build/jelly`. Now
you can learn [how to use jelly](./how-to-use.html).
