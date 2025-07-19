#include "../s21_string.h"

/**
 * @brief Copies a block of memory from source to destination. This function
 * copies n bytes from the memory block pointed to by src to the memory block
 * pointed to by dest. The memory blocks must not overlap.
 * @param dest Pointer to the destination memory block.
 * @param src Pointer to the source memory block.
 * @param n Number of bytes to be copied.
 * @return A pointer to the destination memory block.
 */
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  s21_size_t i;
  char *cdest = (char *)dest;
  const char *csrc = (char *)src;
  for (i = 0; i < n; ++i) *cdest++ = *csrc++;

  return dest;
}