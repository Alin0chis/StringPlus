#include "../s21_string.h"

/**
 * @brief Searches for the first occurrence of a character in a string.
 *
 * This function searches for the first occurrence of the character specified by
 * c in the string str. It returns a pointer to the first occurrence of the
 * character (if found, or a null pointer if the character is not found.
 * @param str The string to search in.
 * @param c The character to search for.
 * @return A pointer to the first occurrence of the character if found, or a
 * null pointer if not found. */
char* s21_strchr(const char* str, s21_size_t c) {
  s21_size_t i = 0;
  while ((str[i] != '\0') && (str[i] != (char)c)) i++;
  return str[i] == '\0' ? (void*)0 : (char*)(str + i);
}
