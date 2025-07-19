#ifndef _SRC_S21_SSCANF_H_
#define _SRC_S21_SSCANF_H_

#include "s21_string.h"

typedef struct {
  int width;
  int flag_w;
  int flag_minus;
  int count_s;
  int flag_star;
  int flag_plus;
  int number_systems;
  char length;
  char u;
} Flags;

int skip_sym(const char* str, const char* sym);
const char* get_width_(const char* format, Flags* flags);
const char* set_flags(const char* format, Flags* flags);
char set_len(char size, char spec);
const char* set_sys(const char* str, Flags* flags);
const char* integer(char spec, const char* str, void* dest, char size,
                    int* count_success, Flags flags);
const char* unsign(char spec, const char* str, void* arg, char size,
                   int* count_success, Flags flags);
const char* parser_(const char* str, const char* string, const char* format,
                    Flags flags, void* arg, int* count_success);
const char* function_p(const char* str, Flags flags, void* arg, char size,
                       int* count_success, char spec);
char get_char_num_q(char c);
const char* function_d_i(const char* str, Flags flags, long long* num);
const char* function_s(const char* str, Flags flags, void* arg, int* count_res);
const char* function_f(const char* str, Flags flags, void* arg, int* count_res,
                       char size);
const char* func_f(const char* str, Flags flags, long double* num,
                   int* count_res);
int is_nan_or_inf_scan(const char* str, long double* num);
#endif  // SRC_S21_SSCANF_H_
