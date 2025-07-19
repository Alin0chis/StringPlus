#include "../s21_string.h"

/**
 * @brief Searches for the first occurrence of a substring in a string. This
 * function searches the null-terminated string haystack for the first
 * occurrence of the null-terminated string needle. It returns a pointer to the
 * beginning of the first occurrence of the substring, or NULL if the substring
 * is not found.
 * @param haystack Pointer to the null-terminated string to be searched.
 * @param needle Pointer to the null-terminated string to search for.
 * @return A pointer to the beginning of the first occurrence of the substring,
 * or NULL if the substring is not found.
 */
char* s21_strstr(const char* haystack, const char* needle) {
  char* result = (void*)0;
  if (*needle == '\0') {
    result = (char*)haystack;
  }
  for (s21_size_t i = 0; i < s21_strlen(haystack); i++) {
    if (*(haystack + i) == *needle) {
      char* ptr = s21_strstr(haystack + i + 1, needle + 1);
      result = (ptr) ? ptr - 1 : (void*)0;
      break;
    }
  }
  return result;
}
