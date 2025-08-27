#!/bin/sh

set -eu

CC=${CC:-gcc}
LD=${LD:-gcc}
BUILD="build"
SOURCES="src/main.c src/fs.c src/str.c src/kvdict.c src/frontmatter.c"
OBJECTS=$(echo "$SOURCES" | sed "s|\([^ ]*\)\.c|$BUILD/\1.c.o|g")
CFLAGS="-Isrc/ -std=c99 -Wall -Wextra -Wshadow -pedantic -ffast-math"
LFLAGS="$(pkg-config --libs libcmark)"
CMD=${1:-}

mkdir -p $BUILD "$BUILD/.ccache"

if [ "$CMD" = "clean" ]; then
	rm -rf $BUILD
	exit 0
elif [ "$CMD" = "bear" ]; then
	bear --append --output "$BUILD/compile_commands.json" -- $0 # github.com/rizsotto/Bear
	exit 0
elif [ "$CMD" = "release" ]; then
	CFLAGS="$CFLAGS -O3 -fdata-sections -ffunction-sections"
	LFLAGS="$LFLAGS -Wl,--gc-sections"
elif [ "$CMD" = "" ]; then
	CFLAGS="$CFLAGS -O0 -g -fsanitize=address,undefined"
	LFLAGS="$LFLAGS -fsanitize=address,undefined"
elif [ "$CMD" = "serve" ]; then
	find www/ | entr -rs "./build/jelly && python3 -m http.server -d www-build/ 4000"
	exit 0
elif [ "$CMD" = "deploy" ]; then
	./build/jelly
	minhtml --keep-closing-tags --minify-css --minify-js --keep-spaces-between-attributes --keep-input-type-text-attr --keep-html-and-head-opening-tags $(find www-build/ -type f -name "*.html") >/dev/null # https://github.com/wilsonzlin/minify-html/
	npx surge www-build/ "https://jellyc.surge.sh"
	exit 0
elif [ "$CMD" ]; then
	echo "Invalid command '$CMD', Available commands are: clean/bear/release/serve/deploy or none to just build."
	exit 1
fi

if ! [ -x "$(command -v ccache)" ]; then CCACHE=""; else CCACHE="ccache"; fi
if [ -x "$(command -v mold)" ]; then LFLAGS="$LFLAGS -fuse-ld=mold"; fi

export CCACHE_DIR="$BUILD/.ccache"

echo "$SOURCES 0" | tr ' ' '\n' | while read source; do
	if [ "$source" = "0" ]; then wait; exit 0; fi
	echo "Compiling $source"
	mkdir -p "$(dirname "$BUILD/$source.o")"
	"$CCACHE" "$CC" $CFLAGS -c "$source" -o "$BUILD/$source.o" &
done

"$LD" $OBJECTS $LFLAGS -o "$BUILD/jelly"
echo "  Linking $BUILD/jelly"
