# Kstd

Kstd is the standard library for Kelyra. Its pure Kelyra modules are:

- `std.ascii`: ASCII byte classification and case conversion
- `std.math.integer`: basic `i64` integer utilities
- `std.math.basic`: basic `f32` and `f64` utilities

The current `std.io` module provides:

- `print`, `println`, `eprint`, and `eprintln`
- `newline` and `flush`
- `print_int` and `read_int`
- `read_line`, backed by a thread-local 4096-byte buffer

`std.alloc` wraps the host allocator and byte-level memory access:

- `allocate`, `reallocate`, and `release`
- `copy` and `fill`
- `load` and `store` for one indexed byte
- `is_null`, so allocation failure is observable without a null literal

`std.string` builds an owned, mutable byte string on top of `std.alloc`:

```kelyra
import std.string;

let text = std.string.String("hello");
text.append(", world");
text.append_byte(33);
text.length();          // 13
text.at(0);             // 104
text.set(0, 72);
text.equals("Hello, world!");  // true
text.data_pointer();    // *c.char, null terminated
// deinit releases the buffer at the end of the scope
```

`String` has a single constructor — Kelyra has no overloading — taking a
`*c.char`; use `String("")` for an empty value, or `clear`/`assign`. It cannot be
copied or returned by value, so pass it as `*String`.

Each Kelyra module keeps its own small C runtime (`src/std/io.c`,
`src/std/alloc.c`, `src/std/string.c`). Kstd itself is built by Kelp using the
checked-in `kelp.toml`, which declares it as a `library` whose entry
`src/kstd.kly` imports every public module. The boundary between pure library
code, platform code, and C compatibility is documented in
[`doc/glibc-scope.md`](doc/glibc-scope.md).
From the repository root:

```sh
. ./env.sh
kelp check
kelp build
kelp test
```

`kelp build` produces the library object `.kelp/build/kstd.o`; a program links it
instead of recompiling the standard modules.

`env.sh` aliases the sibling Kelp and Kelyra build paths for the current shell.

A consumer depends on the library and links its object. With Kelp, point a
dependency at this repository and let Kelp pass `--external-path` and
`--link-input`; to compile directly, also pass the C runtimes the object uses:

```sh
kelyra --emit-exe \
  --module-path=kstd/src --external-path=kstd/src \
  --link-input=kstd/.kelp/build/kstd.o \
  --c-source=kstd/src/std/io.c \
  --c-source=kstd/src/std/alloc.c \
  --c-source=kstd/src/std/string.c \
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
sh tests/string.sh
sh tests/core.sh
```
