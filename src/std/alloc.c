#include "alloc.h"

#include <stdlib.h>
#include <string.h>

unsigned char *kstd_alloc(size_t size) { return malloc(size); }

unsigned char *kstd_realloc(unsigned char *pointer, size_t size) {
  return realloc(pointer, size);
}

void kstd_free(unsigned char *pointer) { free(pointer); }

void kstd_copy(unsigned char *destination, const unsigned char *source,
               size_t count) {
  memcpy(destination, source, count);
}

void kstd_fill(unsigned char *destination, unsigned char value, size_t count) {
  memset(destination, value, count);
}

unsigned char kstd_load(const unsigned char *pointer, size_t index) {
  return pointer[index];
}

void kstd_store(unsigned char *pointer, size_t index, unsigned char value) {
  pointer[index] = value;
}

int kstd_is_null(const unsigned char *pointer) { return pointer == NULL; }
