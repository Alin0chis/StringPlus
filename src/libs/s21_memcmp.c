#include "../s21_string.h"

/**
 * @brief Compares the first n bytes of two memory blocks. This function
 * compares the first n bytes of the memory blocks pointed to by str1 and str2.
 * It returns an integer greater than, equal to, or less than 0, depending on
 * whether str1 is greater than, equal to, or less than str2 respectively.
 * @param str1 Pointer to the first memory block to be compared.
 * @param str2 Pointer to the second memory block to be compared.
 * @param n Number of bytes to be compared.
 * @return An integer greater than, equal to, or less than 0, if the first n
 * bytes of str1 are greater than, equal to, or less than the first n bytes of
 * str2 respectively.
 */

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  unsigned char *s1 = (unsigned char *)str1;
  unsigned char *s2 = (unsigned char *)str2;
  int dif = 0;
  for (s21_size_t i = 0; i < n; i++) {
    dif = *s1 - *s2;
    if (*s1 != *s2) break;
    s1++;
    s2++;
  }

  return dif;
}