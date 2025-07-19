#include "../s21_string.h"

/**
 * @brief Concatenates the first n characters of the source string to the
 * destination string. This function appends the first n characters of the
 * string pointed to by src to the end of the string pointed to by dest. The
 * resulting string in dest is null-terminated.
 *
 * @param dest Pointer to the destination string, which should be large enough
 * to hold the concatenated result.
 * @param src Pointer to the source string.
 * @param n Maximum number of characters to be appended from src.
 * @return Pointer to the destination string.
 */
char* s21_strncat(char* dest, const char* src, s21_size_t n) {
  s21_size_t i = 0;
  s21_size_t j = 0;
  while (dest[i] != '\0') {
    i++;
  }
  while (src[j] != '\0' && j < n) {
    dest[i] = src[j];
    i++;
    j++;
  }
  dest[i] = '\0';
  return dest;
}