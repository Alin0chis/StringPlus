#include "../s21_string.h"

/**
 * @brief Searches for the first occurrence of a character in a block of memory.
 * This function searches within the first n bytes of the memory block pointed
 * to by str for the first occurrence of the character c (interpreted as an
 * unsigned char).
 *
 * @param str Pointer to the memory block to be searched.
 * @param c Value to be searched. The value is passed as an int, but the
 * function searches for the unsigned char conversion of this value.
 * @param n Number of bytes to be searched.
 * @return A pointer to the first occurrence of the character in str, or NULL if
 * the character is not found within the given memory block.
 *
 */
void *s21_memchr(const void *str, int c, s21_size_t n) {
  s21_size_t i;
  char *res = s21_NULL;
  for (i = 0; i < n; i++)
    if (*((char *)str + i) == (char)c) {
      res = (char *)str + i;
      break;
    }
  return res;
}