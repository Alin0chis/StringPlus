#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t res = 0;

  for (const char *i = str1; *i; i++) {
    unsigned short flag = 0;
    for (const char *j = str2; *j; j++) {
      if (*i == *j) {
        flag = 1;
        break;
      }
    }
    if (flag) {
      break;
    } else {
      res++;
    }
  }
  return res;
}