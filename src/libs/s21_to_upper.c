#include "../s21_string.h"

/**
 * @brief Converts a string to uppercase.
 * This function converts the null-terminated string str to uppercase. It
 * allocates memory for a new string and returns a pointer to the uppercase
 * version of the string. If str is NULL, the function returns NULL.
 * @param str Pointer to the null-terminated string to be converted.
 * @return A pointer to the uppercase version of the string, or NULL if str is
 * NULL.
 */
void *s21_to_upper(const char *str) {
  char *result = s21_NULL;
  if (str != s21_NULL) {
    result = (char *)malloc(sizeof(char) * s21_strlen(str));
    for (s21_size_t i = 0; i < s21_strlen(str); i++) {
      if (str[i] > 96 && str[i] < 123)
        result[i] = str[i] - 32;
      else
        result[i] = str[i];
    }
    result[s21_strlen(str)] = '\0';
  }
  return result;
}