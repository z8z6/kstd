#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
kelp=${KELP:-$root/../kelp/build/kelp}
output=$root/build/kstd-io
stdout=${TMPDIR:-/tmp}/kstd-io-$$.stdout
stderr=${TMPDIR:-/tmp}/kstd-io-$$.stderr
trap 'rm -f "$stdout" "$stderr"' EXIT HUP INT TERM

cd "$root"
"$kelp" build
printf 'hello from input\n' | "$output" >"$stdout" 2>"$stderr"
test ! -s "$stderr"
test "$(cat "$stdout")" = "answer = 42
hello from input"
