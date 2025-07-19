#include "../s21_string.h"

/**
 * @brief Copies a string from source to destination with a specified maximum
 * length. This function copies at most n characters from the null-terminated
 * string pointed to by src to the character array pointed to by dest. If the
 * length of src is less than n, the remaining characters in dest are padded
 * with null characters until a total of n characters have been written.
 * @param dest Pointer to the destination character array.
 * @param src Pointer to the source null-terminated string.
 * @param n Maximum number of characters to be copied.
 * @return Pointer to the original destination character array.
 */
char* s21_strncpy(char* dest, const char* src, s21_size_t n) {
  char* origDest = dest;
  s21_size_t i = 0;
  while (*src != '\0' && i < n) {
    *dest = *src;
    src++;
    dest++;
    i++;
  }
  while (i < n) {
    *dest = '\0';
    dest++;
    i++;
  }
  return origDest;
}
