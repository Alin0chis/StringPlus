#include "../s21_string.h"

/**
 * *@brief Inserts the string str into the string src at the specified start
 * index. This function creates a new string by inserting the string str into
 * the string src at the specified start index. The resulting string is
 * dynamically allocated using malloc and should be freed by the caller when no
 * longer needed.
 *
 * @param src Pointer to the source string.
 * @param str Pointer to the string to be inserted.
 * @param start_index Index at which the string str should be inserted into
 * src.
 * @return Pointer to the newly created string, or S21_NULL if the insertion is
 * not possible.
 * */
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = s21_NULL;
  if (!((str == s21_NULL && src == s21_NULL) ||
        start_index > s21_strlen(src))) {
    result = (char *)malloc(sizeof(char) * (s21_strlen(str) + s21_strlen(src)));
    for (s21_size_t i = 0; i < start_index; i++) {
      result[i] = src[i];
    }
    for (s21_size_t i = start_index; i < start_index + s21_strlen(str); i++) {
      result[i] = str[i - start_index];
    }
    for (s21_size_t i = start_index + s21_strlen(str);
         i < s21_strlen(str) + s21_strlen(src); i++) {
      result[i] = src[i - s21_strlen(str)];
    }
    result[s21_strlen(str) + s21_strlen(src)] = '\0';
  }
  return result;
}
