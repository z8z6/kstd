#include "string.h"

#include <string.h>

size_t kstd_text_length(const char *text) {
  return text == NULL ? 0 : strlen(text);
}

void kstd_text_copy_at(unsigned char *data, size_t offset, const char *source,
                       size_t count) {
  memcpy(data + offset, source, count);
}

int kstd_text_equals(const unsigned char *data, size_t length,
                     const char *text) {
  const size_t Expected = kstd_text_length(text);
  if (length != Expected)
    return 0;
  return length == 0 || memcmp(data, text, length) == 0;
}

char *kstd_text_data(unsigned char *data) { return (char *)data; }
