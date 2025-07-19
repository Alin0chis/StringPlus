#include "../s21_string.h"

/**
 * @brief Calculates the length of a null-terminated string.
 * This function counts the number of characters in the string pointed to by str
 * before the null character '\0' is encountered.
 * @param str Pointer to the null-terminated string.
 * @return The length of the string, excluding the null character.
 */
s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  if (str) {
    for (int i = 0; str[i]; i++) {
      len++;
    }
  }
  return len;
}
