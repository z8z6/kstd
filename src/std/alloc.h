#ifndef KSTD_ALLOC_H
#define KSTD_ALLOC_H

// Declared here so the header is self-contained for the Kelyra C importer,
// which does not resolve the system headers.
typedef __SIZE_TYPE__ size_t;

unsigned char *kstd_alloc(size_t size);
unsigned char *kstd_realloc(unsigned char *pointer, size_t size);
void kstd_free(unsigned char *pointer);
void kstd_copy(unsigned char *destination, const unsigned char *source,
               size_t count);
void kstd_fill(unsigned char *destination, unsigned char value, size_t count);
unsigned char kstd_load(const unsigned char *pointer, size_t index);
void kstd_store(unsigned char *pointer, size_t index, unsigned char value);
int kstd_is_null(const unsigned char *pointer);

#endif
