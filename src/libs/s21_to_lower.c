#include "../s21_string.h"

/**
 * @brief Converts a string to lowercase.
 * This function converts the null-terminated string str to lowercase. It
 * allocates memory for a new string and returns a pointer to the lowercase
 * version of the string. If str is NULL, the function returns NULL.
 * @param str Pointer to the null-terminated string to be converted.
 * @return A pointer to the lowercase version of the string, or NULL if str is
 * NULL.
 */
void *s21_to_lower(const char *str) {
  char *result = s21_NULL;
  if (str != s21_NULL) {
    result = (char *)malloc(sizeof(char) * s21_strlen(str));
    for (int i = 0; i < (int)s21_strlen(str); i++) {
      result[i] = str[i];
      if ((signed char)str[i] >= 65 && (signed char)str[i] <= 90) {
        result[i] = str[i] + 32;
      }
    }
    result[s21_strlen(str)] = '\0';
  }
  return result;
}
