#include "../s21_string.h"

int check(char t, const char *array) {
  int result = 1;

  for (s21_size_t i = 0; i < s21_strlen(array); i++) {
    if (array[i] == t) {
      result = 0;
      break;
    }
  }

  return result;
}

/**
 * @brief This function trims leading and trailing characters in a given string
 * based on the specified trim characters.
 *
 * @param src The source string to be trimmed.
 * @param trim_chars The string of characters to be used for
 * trimming. If trim_chars is NULL or an empty string, a space character will be
 * used as the default trim character.
 *
 * @return (void*): A pointer to the trimmed string. If src is NULL, NULL is
 * returned.
 */
void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;
  int size_l = 0;
  int size_r = 0;
  int len_s = s21_strlen(src) - 1;
  if (src != NULL) {
    result = malloc(sizeof(char));
    result[0] = '\0';
    if (trim_chars == s21_NULL || trim_chars[0] == '\0') {
      trim_chars = " ";
    }
    for (s21_size_t i = 0; i < s21_strlen(src) - 1; i++) {
      if (!check(src[i], trim_chars)) {
        size_l++;
      } else {
        break;
      }
    }
    for (s21_size_t i = len_s; i > 0; i--) {
      if (!check(src[i], trim_chars)) {
        size_r++;
      } else {
        break;
      }
    }
    for (s21_size_t i = size_l; i < (s21_strlen(src) - size_r); i++) {
      result = realloc(result, (s21_strlen(result) + 1) * sizeof(char));
      result[i - size_l] = src[i];
    }
    result[s21_strlen(src) - size_l - size_r] = '\0';
  }
  return result;
}