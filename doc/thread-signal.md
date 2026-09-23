# Thread and signal system APIs

`std.thread` and `std.signal` provide low-level, freestanding-safe OS calls on
Linux x86-64 and Windows x64. They contain no C implementation or CRT calls.
Platform selection uses `@cfg(os="...", arch="x86_64")`.

| Module | Linux x86-64 | Windows x64 |
| --- | --- | --- |
| `std.thread.current_id` | `gettid`, `i64` | `GetCurrentThreadId`, widened to `i64` |
| `std.thread.yield_now` | `sched_yield`, zero or `-errno` | `SwitchToThread`, returns zero even if no peer was scheduled |
| `std.thread.wait_word` | private futex wait, zero or `-errno` | `WaitOnAddress`, zero or negative `GetLastError` |
| `std.thread.wake_one` / `wake_all` | private futex wake, count or `-errno` | `WakeByAddressSingle` / `WakeByAddressAll`, returns zero |
| `std.signal` | `send_process` (`kill`) and `send_thread` (`tgkill`), zero or `-errno` | `send_console_event` (`GenerateConsoleCtrlEvent`) and `ignore_control_c` (`SetConsoleCtrlHandler(NULL, ...)`) |

`wait_word` compares one aligned 32-bit word and waits while it has the
expected value. Callers must change the word with an atomic operation *before*
waking other threads, and must recheck it in a loop after waking: spurious
wakeups and races are allowed. This module does not yet provide atomic writes,
a mutex, or a thread creation/join API, so it is a building block rather than
a safe concurrency abstraction. Linux `wait_word` returns `-11` (`EAGAIN`) if
the value already differs. Windows `wait_word` currently waits indefinitely.
The public `std.thread` signatures use `i64` on both platforms; Linux `wake_*`
reports a nonnegative wake count whereas Windows returns zero because its API
does not report the count. The platform-specific modules retain their native
return signatures.

Linux signal numbers are kernel signal numbers; signal zero probes process or
thread existence/permission without delivery. Windows console control events
are **not** POSIX signals. `send_console_event` only accepts event 0 (`CTRL_C`)
or 1 (`CTRL_BREAK`), and requires a suitable shared console/process group.
`ignore_control_c` toggles a process-wide, inheritable state; use with care.

These APIs intentionally do not claim to implement `spawn`, `join`, or a
signal-handler callback. Kelyra currently rejects function-value parameters
on `@extern` declarations, which prevents a direct `CreateThread` or
`SetConsoleCtrlHandler` callback signature. A Linux `clone` implementation
also needs a sound stack, TLS and startup trampoline contract. Those features
need language/runtime design before they can be exposed safely.

`sh tests/thread_signal.sh` runs a freestanding Linux smoke test. A Windows
cross build can use `--target=x86_64-pc-windows-msvc` and link both
`Kernel32.lib` and `Synchronization.lib`. The address-wait functions are
imported from the Windows synchronization API set, not Kernel32's import
library. The Windows smoke test was linked and run from WSL on Windows x64
with both import libraries and returned zero; `tests/thread_signal.sh` remains
Linux-only because its executable format and launcher are native to Linux.

The main kstd library imports `std.thread`. Its Kelp manifest declares
`windows-import-libraries = ["Synchronization.lib"]`, which Kelp propagates
to Windows consumers of the library. Direct compiler users must still pass
the import library explicitly when linking a program that includes kstd.
