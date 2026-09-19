#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int write_text(FILE *stream, const char *text, int newline) {
  if (text == NULL || fputs(text, stream) == EOF)
    return -1;
  if (newline && fputc('\n', stream) == EOF)
    return -1;
  return 0;
}

int kstd_io_print(const char *text) { return write_text(stdout, text, 0); }

int kstd_io_println(const char *text) { return write_text(stdout, text, 1); }

int kstd_io_eprint(const char *text) { return write_text(stderr, text, 0); }

int kstd_io_eprintln(const char *text) { return write_text(stderr, text, 1); }

int kstd_io_newline(void) { return fputc('\n', stdout) == EOF ? -1 : 0; }

int kstd_io_print_int(long long value) {
  return fprintf(stdout, "%lld", value) < 0 ? -1 : 0;
}

char *kstd_io_read_line(void) {
  static _Thread_local char buffer[4096];
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    buffer[0] = '\0';
    return buffer;
  }
  buffer[strcspn(buffer, "\r\n")] = '\0';
  return buffer;
}

long long kstd_io_read_int(void) {
  const char *text = kstd_io_read_line();
  return strtoll(text, NULL, 10);
}

int kstd_io_flush(void) { return fflush(stdout) == EOF ? -1 : 0; }
