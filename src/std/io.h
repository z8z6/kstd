#ifndef KSTD_IO_H
#define KSTD_IO_H

#ifdef __cplusplus
extern "C" {
#endif

int kstd_io_print(const char *text);
int kstd_io_println(const char *text);
int kstd_io_eprint(const char *text);
int kstd_io_eprintln(const char *text);
int kstd_io_newline(void);
int kstd_io_print_int(long long value);
char *kstd_io_read_line(void);
long long kstd_io_read_int(void);
int kstd_io_flush(void);

#ifdef __cplusplus
}
#endif

#endif
