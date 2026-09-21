#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
kelp=${KELP:-$root/../kelp/build/kelp}
compiler=${KELYRA:-$root/../kelyra/build/bin/kelyra}
output=$root/build/kstd-io
stdout=${TMPDIR:-/tmp}/kstd-io-$$.stdout
stderr=${TMPDIR:-/tmp}/kstd-io-$$.stderr
trap 'rm -f "$stdout" "$stderr"' EXIT HUP INT TERM

cd "$root"
mkdir -p "$root/build"
# Build the library object, then link the example against it: the example only
# declares the std modules and resolves their definitions from the object. The
# object references every C runtime the library carries.
"$kelp" build
"$compiler" --emit-exe \
  --module-path="$root/src" --external-path="$root/src" \
  --link-input="$root/.kelp/build/kstd.o" \
  --c-source="$root/src/std/io.c" \
  --c-source="$root/src/std/alloc.c" \
  --c-source="$root/src/std/string.c" \
  -o "$output" "$root/src/io_example.kly"
printf 'hello from input\n' | "$output" >"$stdout" 2>"$stderr"
test ! -s "$stderr"
test "$(cat "$stdout")" = "answer = 42
hello from input"
