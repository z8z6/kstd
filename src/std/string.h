#ifndef KSTD_STRING_H
#define KSTD_STRING_H

// Declared here so the header is self-contained for the Kelyra C importer,
// which does not resolve the system headers.
typedef __SIZE_TYPE__ size_t;

size_t kstd_text_length(const char *text);
void kstd_text_copy_at(unsigned char *data, size_t offset, const char *source,
                       size_t count);
int kstd_text_equals(const unsigned char *data, size_t length,
                     const char *text);
char *kstd_text_data(unsigned char *data);

#endif
