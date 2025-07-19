#include "../s21_string.h"

/**
 * @brief Searches a string for any of a set of characters. This function
 * searches the null-terminated string str1 for any character that matches any
 * character in the null-terminated string str2. It returns a pointer to the
 * first occurrence of a matching character, or NULL if no match is found.
 * @param str1 Pointer to the null-terminated string to be searched.
 * @param str2 Pointer to the null-terminated string containing the characters
 * to search for.
 * @return A pointer to the first occurrence of a matching character, or NULL if
 * no match is found.
 */
char *s21_strpbrk(const char *str1, const char *str2) {
  const char *res = s21_NULL;
  s21_size_t s1_len = s21_strlen(str1);
  s21_size_t s2_len = s21_strlen(str2);

  for (s21_size_t i = 0; i < s1_len; i++) {
    for (s21_size_t j = 0; j < s2_len; j++) {
      if (str1[i] == str2[j]) {
        res = str1 + i;
        break;
      }
    }
    if (res != s21_NULL) {
      break;
    }
  }
  return (char *)res;
}
