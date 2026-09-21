# Kstd

Kstd is the standard library for Kelyra. Its first pure Kelyra modules are:

- `std.ascii`: ASCII byte classification and case conversion
- `std.math.integer`: basic `i64` integer utilities
- `std.math.basic`: basic `f32` and `f64` utilities

The current `std.io` module provides:

- `print`, `println`, `eprint`, and `eprintln`
- `newline` and `flush`
- `print_int` and `read_int`
- `read_line`, backed by a thread-local 4096-byte buffer

The Kelyra module is `src/std/io.kly`; its small C runtime is
`src/std/io.c`. Kstd itself is built by Kelp using the checked-in `kelp.toml`,
which declares it as a `library` whose entry `src/kstd.kly` imports every public
module. The boundary between pure library code, platform code, and C
compatibility is documented in [`doc/glibc-scope.md`](doc/glibc-scope.md).
From the repository root:

```sh
. ./env.sh
kelp check
kelp build
kelp test
```

`kelp build` produces the library object `build/kstd.o`; a program links it
instead of recompiling the standard modules.

`env.sh` aliases the sibling Kelp and Kelyra build paths for the current shell.

A consumer depends on the library and links its object. With Kelp, point a
dependency at this repository and let Kelp pass `--external-path` and
`--link-input`; to compile directly:

```sh
kelyra --emit-exe \
  --module-path=kstd/src --external-path=kstd/src \
  --link-input=kstd/build/kstd.o --c-source=kstd/src/std/io.c \
  -o app main.kly
```

Then import it from Kelyra:

```kelyra
import std.io;

pub fn main() -> i32 {
  std.io.println("hello");
  std.io.print_int(42);
  std.io.newline();
  return 0;
}
```

Test:

```sh
kelp test
sh tests/io.sh
sh tests/core.sh
```
