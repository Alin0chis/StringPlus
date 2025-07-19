#include "../s21_string.h"

/**
 * @brief Sets each byte of the specified memory block to the given value.
 *
 * This function copies the value of c (interpreted as an unsigned char) into
 * each of the first n bytes of the object pointed to by str.
 *
 * @param str Pointer to the memory block to be filled.
 * @param c Value to be set. The value is passed as an int, but the function
 * fills the block of memory using the unsigned char conversion of this value.
 * @param n Number of bytes to be set to the value.
 * @return A pointer to the memory block str.
 */
void *s21_memset(void *str, int c, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    *((char *)str + i) = c;
  }
  return str;
}