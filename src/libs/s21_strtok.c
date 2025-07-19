#include "../s21_string.h"

/**
 * @brief Calculates the length of the initial substring that consists of
 * characters from a given set.
 * This function calculates the length of the initial substring of the
 * null-terminated string str1, consisting of characters that are present in the
 * null-terminated string str2. It returns the number of characters in the
 * initial substring.
 * @param str1 Pointer to the null-terminated string to be searched.
 * @param str2 Pointer to the null-terminated string containing the set of
 * characters.
 * @return The length of the initial substring consisting of characters from the
 * set.
 */
s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t counter = 0;
  for (const char *a = str1; *a; a++) {
    unsigned short was = 0;
    for (const char *b = str2; *b; b++) {
      if (*a == *b) {
        counter++;
        was = 1;
        break;
      }
    }
    if (!was) {
      break;
    }
  }
  return counter;
}

/**
 * @brief Breaks a string into a sequence of tokens.
 * This function breaks the null-terminated string str into a sequence of
 * tokens, where each token is separated by one or more characters from the
 * null-terminated string delim. On the first call to strtok, the string to be
 * parsed should be specified in str. In subsequent calls, str should be set to
 * NULL. The function maintains a static pointer to the last token found, and
 * returns a pointer to the next token in the sequence. If no more tokens are
 * found, NULL is returned.
 * @param str Pointer to the null-terminated string to be parsed.
 * @param delim Pointer to the null-terminated string containing the delimiter
 * characters.
 * @return A pointer to the next token found in the string, or NULL if no more
 * tokens are found.
 */
char *s21_strtok(char *str, const char *delim) {
  static int next_null, tok;
  static s21_size_t ind;
  static char *max_ptr;
  static char *addr;

  if (str) {
    tok = 1;
    ind = 0, next_null = 0;
    addr = str + s21_strspn(str, delim);
    max_ptr = str + s21_strlen(str);
  }
  char *res = s21_NULL;
  if (!(addr >= max_ptr || next_null)) {
    int non_delim = 1;
    for (int i = 0; addr[i]; i++) {
      s21_size_t step = s21_strspn(addr + i, delim);
      if (step) {
        non_delim = 0;
        for (s21_size_t j = i; j < i + step; j++) addr[j] = '\0';
        ind = step + i;
        break;
      }
    }
    if (tok == 1 && non_delim) {
      next_null = 1;
    }
    res = addr;
    addr += ind;
  }
  ++tok;
  return res;
}