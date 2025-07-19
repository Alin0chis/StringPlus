#include "../s21_string.h"

/**
 * @brief Searches for the last occurrence of a character in a string. This
 * function searches the null-terminated string str for the last occurrence of
 * the character c. It returns a pointer to the last occurrence of the
 * character, or NULL if the character is not found.
 * @param str Pointer to the null-terminated string to be searched.
 * @param c Character to be searched.
 * @return A pointer to the last occurrence of the character in str, or NULL if
 * the character is not found.
 */
char *s21_strrchr(const char *str, int c) {
  char *res = s21_NULL;
  s21_size_t s_len = s21_strlen(str);
  for (s21_size_t i = 0; i <= s_len; i++) {
    if (str[i] == c) {
      res = ((char *)str) + i;
    }
  }
  return res;
}