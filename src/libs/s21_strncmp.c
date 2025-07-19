#include "../s21_string.h"

/**
 * @brief Compares the first n characters of two strings lexicographically. This
 * function compares the first n characters of the null-terminated strings
 * pointed to by str1 and str2. The comparison is done lexicographically (the
 * characters are compared based on their ASCII values).
 *
 * @param str1 Pointer to the first null-terminated string to be compared.
 * @param str2 Pointer to the second null-terminated string to be compared.
 * @param n Maximum number of characters to be compared.
 * @return An integer greater than, equal to, or less than 0, if the first n
 * characters of str1  are greater than, equal to, or less than the first n
 * characters of str2 respectively.
 */
int s21_strncmp(const char* str1, const char* str2, s21_size_t n) {
  int res = 0;

  if (str1 && str2 && n > 0) {
    for (s21_size_t i = 0; i < n; i++) {
      if (str1[i] == '\0' || str1[i] != str2[i]) {
        res = str1[i] - str2[i];
        break;
      }
    }
  }
  return res;
}
