# Kstd

Kstd is the standard library for Kelyra. The initial `std.io` module provides:

- `print`, `println`, `eprint`, and `eprintln`
- `newline` and `flush`
- `print_int` and `read_int`
- `read_line`, backed by a thread-local 4096-byte buffer

The Kelyra module is `src/std/io.kly`; its small C runtime is
`src/std/io.c`. Kstd itself is built by Kelp using the checked-in `kelp.toml`.
From the repository root:

```sh
. ./env.sh
kelp check
kelp build
kelp run
```

`env.sh` aliases the sibling Kelp and Kelyra build paths for the current shell.

Until Kelp gains package installation, copy `src/std` into a project's `src`
directory and add the runtime to its `kelp.toml`:

```toml
[build]
c-sources = ["src/std/io.c"]
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
```
