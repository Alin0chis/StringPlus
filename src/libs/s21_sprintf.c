#include "../s21_sprintf.h"

/**
 * отправляет форматированный вывод в строку, на которую указывает str.
 * @param str Это С-строка, которую функция обрабатывает в качестве источника
 * для извлечения данных
 * @param format это С-строка, содержащая один или несколько следующих
 * элементов: пробельный символ, непробельный символ и спецификаторы формата.
 * Спецификатор формата для печатающих функций следует прототипу:
 * %[флаги][ширина][.точность][длина]спецификатор
 * @param ... представляет другие параметры, такие как ширина, точность, длина и
 * спецификатор.
 * @return Функция sprintf имеет возвращаемый целочисленный тип. В случае успеха
 * функция возвращает количество символов, записанных в буфер, за исключением
 * завершающего нулевого символа.
 * В случае ошибки функция возвращает отрицательное
 * целочисленное значение
 */
int s21_sprintf(char *str, const char *format, ...) {
  char specifiers[] = "diuoxXcsnpfFeEgG%";
  char *src = str;
  va_list arguments;
  va_start(arguments, format);
  while (*format) {
    if (*format == '%') {
      format++;
      Spec specs = {0};
      specs.number_system = 10;
      format = set_specs(&specs, format, &arguments);
      while (!s21_strchr(specifiers, *format)) format++;
      str = parser(str, src, format, specs, &arguments);
    } else {
      *str = *format;
      str++;
    }
    format++;
  }
  *str = '\0';
  va_end(arguments);
  return (str - src);
}

const char *get_specs(const char *format, Spec *specs) {
  while (format) {
    if (*format == '+')
      specs->plus = 1;
    else if (*format == '-')
      specs->minus = 1;
    else if (*format == '#')
      specs->hash = 1;
    else if (*format == ' ')
      specs->space = 1;
    else if (*format == '0')
      specs->zero = 1;
    else
      break;
    format++;
  }
  specs->space = (specs->space && !specs->plus);
  specs->zero = (specs->zero && !specs->minus);
  return format;
}

const char *get_width(const char *format, int *width, va_list *arguments) {
  *width = 0;
  if (*format == '*') {
    *width = va_arg(*arguments, int);
    format++;
  }
  while (format) {
    if ('0' <= *format && *format <= '9') {
      *width *= 10;
      *width += *format - '0';
    } else
      break;
    format++;
  }
  return format;
}

const char *set_specs(Spec *specs, const char *format, va_list *arguments) {
  format = get_specs(format, specs);
  format = get_width(format, &specs->width, arguments);
  if (*format == '.') {
    specs->dot = 1;
    specs->zero = 0;
    format += 1;
    format = get_width(format, &specs->accuracy, arguments);
  }
  if (*format == 'L')
    specs->length = 'L';
  else if (*format == 'l')
    specs->length = 'l';
  else if (*format == 'h')
    specs->length = 'h';
  if (specs->length != 0) format += 1;

  if (specs->width < 0) {
    specs->width = -specs->width;
    specs->minus = 1;
  }
  return format;
}

int add_sym_to_str(char *str, int *i, char symbol) {
  *str = symbol;
  *i += 1;
  return 1;
}

long add_space_plus_minus(Spec specs, long double num, int *i, char *str_to_num,
                          int *flag_to_size, long int *size) {
  long int size_this = *size;
  if (specs.space && num >= 0 && size_this) {
    add_sym_to_str(str_to_num + *i, i, ' ');
    size_this -= 1;
    if (num != 0) *flag_to_size -= 1;
  }
  if (num < 0 && size_this) {
    add_sym_to_str(str_to_num + *i, i, '-');
    size_this -= 1;
    *flag_to_size -= 1;
  }
  if (num > 0 && specs.plus && size_this) {
    add_sym_to_str(str_to_num + *i, i, '+');
    size_this -= 1;
    *flag_to_size -= 1;
  }
  return size_this;
}

int add_buffer_to_string(int width, int *i, char *buffer, char *str) {
  int count = 0;
  for (int j = *i - 1; j >= 0; j--) {
    str += add_sym_to_str(str, &count, *(buffer + j));
  }
  while ((*i < width)) {
    str += add_sym_to_str(str, i, ' ');
    count++;
  }
  return count;
}

char *print_decimal(char *str, Spec specs, va_list *arguments) {
  long int num = 0;
  if (specs.length == 'l') {
    num = (long int)va_arg(*arguments, long int);
  } else if (specs.length == 'h') {
    num = (short)va_arg(*arguments, int);
  } else {
    num = (int)va_arg(*arguments, int);
  }
  s21_size_t size_to_decimal = get_size_to_decimal(&specs, num);
  char *str_to_num = malloc(sizeof(char) * size_to_decimal);
  if (str_to_num) {
    int i = decimal_to_string(specs, num, str_to_num, size_to_decimal);
    str += add_buffer_to_string(specs.width, &i, str_to_num, str);
  }
  if (str_to_num) free(str_to_num);
  return str;
}

s21_size_t get_size_to_decimal(Spec *specs, long int num) {
  s21_size_t result = 0;
  long int copy_num = num;
  if (copy_num < 0) copy_num = -copy_num;
  while (copy_num > 0) {
    copy_num /= 10;
    result++;
  }
  if (copy_num == 0 && result == 0 &&
      (specs->accuracy || specs->width || specs->space)) {
    result++;
  }
  if ((s21_size_t)specs->width > result) result = specs->width;
  if ((s21_size_t)specs->accuracy > result) result = specs->accuracy;
  if (specs->space || specs->plus || num < 0) {
    specs->flag_to_size = 1;
    result++;
  }
  if (result == 0 && copy_num == 0 && !specs->accuracy && !specs->width &&
      !specs->space && !specs->dot)
    result++;
  return result;
}

int invert_num(long int *num_int, long double *num_double) {
  int flag = 0;
  if (*num_int < 0) {
    flag = 1;
    *num_int = -*num_int;
  }
  if (*num_double < 0) {
    flag = 1;
    *num_double = -*num_double;
  }
  return flag;
}

int add_zero_to_str(Spec specs, char *str_to_num, s21_size_t size, int *i,
                    int flag) {
  while (specs.zero && str_to_num && (size - specs.flag_to_size > 0) &&
         (specs.accuracy || flag)) {
    if ((size - specs.flag_to_size == 0) ||
        (size == 1 && specs.flag_to_size == 1))
      break;
    size -= add_sym_to_str(str_to_num + *i, i, '0');
    specs.accuracy--;
  }
  return size;
}

int decimal_to_string(Spec specs, long int num, char *str_to_num,
                      s21_size_t size_to_decimal) {
  long double num_double = 0;
  int flag = invert_num(&num, &num_double);
  int i = 0;
  long int copy_num = num;
  if ((copy_num == 0 && (specs.accuracy || specs.width || specs.space)) ||
      (copy_num == 0 && !specs.accuracy && !specs.width && !specs.space &&
       !specs.dot)) {
    size_to_decimal -= add_sym_to_str(
        str_to_num + i, &i, get_num_char(copy_num % 10, specs.upper_case));
    copy_num /= 10;
  }
  while (copy_num && str_to_num && size_to_decimal) {
    size_to_decimal -= add_sym_to_str(
        str_to_num + i, &i, get_num_char(copy_num % 10, specs.upper_case));
    copy_num /= 10;
  }
  if (flag) num = -num;
  if (specs.accuracy - i > 0) {
    specs.accuracy -= i;
    specs.zero = 1;
  } else
    flag = 1;
  if (size_to_decimal == 1 && specs.zero == 1 && specs.flag_to_size == 1)
    specs.zero = 0;

  size_to_decimal =
      add_zero_to_str(specs, str_to_num, size_to_decimal, &i, flag);
  int flag_to_size = 0;
  long int size = size_to_decimal;
  size_to_decimal = add_space_plus_minus(specs, (long double)num, &i,
                                         str_to_num, &flag_to_size, &size);
  if (size_to_decimal > 0 && specs.minus == 0) {
    while ((size_to_decimal - specs.flag_to_size > 0) && str_to_num) {
      size_to_decimal -= add_sym_to_str(str_to_num + i, &i, ' ');
    }
  }
  return i;
}

Spec set_number_system(Spec specs, char format) {
  if (format == 'o')
    specs.number_system = 8;
  else if (format == 'x' || format == 'X')
    specs.number_system = 16;
  if (format == 'X') specs.upper_case = 1;
  return specs;
}

char *print_u(char *str, Spec specs, char format, va_list *arguments) {
  unsigned long int num = 0;
  if (format == 'l')
    num = (unsigned long int)va_arg(*arguments, unsigned long int);
  else if (format == 'h')
    num = (unsigned short)va_arg(*arguments, unsigned int);
  else
    num = (unsigned int)va_arg(*arguments, unsigned int);
  s21_size_t size_to_num = get_buf_size_unsigned_decimal(specs, num);
  char *buffer = malloc(sizeof(char) * size_to_num);
  if (buffer) {
    int i = unsigned_decimal_to_string(buffer, specs, num, size_to_num);
    str += add_buffer_to_string(specs.width, &i, buffer, str);
  }
  if (buffer) free(buffer);
  return str;
}

s21_size_t get_buf_size_unsigned_decimal(Spec specs, unsigned long int num) {
  s21_size_t result = 0;
  unsigned long int copy_num = num;
  while (num > 0) {
    num /= specs.number_system;
    result++;
  }
  if (num == 0 && result == 0) {
    result++;
  }
  if (specs.hash && specs.number_system == 16 && copy_num && !specs.accuracy)
    result += 2;
  else if ((specs.hash && specs.number_system == 8 && copy_num &&
            !specs.accuracy))
    result += 1;
  if ((s21_size_t)specs.width > result) result = specs.width;
  if ((s21_size_t)specs.accuracy > result) result = specs.accuracy;
  if (specs.hash && specs.number_system == 16 && copy_num)
    result += 2;
  else if ((specs.hash && specs.number_system == 8 && copy_num))
    result += 1;
  return result;
}

int unsigned_decimal_to_string(char *buffer, Spec specs, unsigned long int num,
                               s21_size_t size_to_num) {
  int i = 0;
  int flag = 0;
  unsigned long int copy_num = num;
  if ((specs.hash && specs.number_system == 8))
    specs.flag_to_size = 1;
  else if (specs.hash && specs.number_system == 16) {
    specs.flag_to_size = 2;
  }
  if (copy_num == 0) {
    size_to_num -= add_sym_to_str(buffer + i, &i, '0');
    copy_num /= 10;
    specs.hash = 0;
  }
  while (copy_num && buffer && size_to_num) {
    size_to_num -= add_sym_to_str(
        buffer + i, &i,
        get_num_char(copy_num % specs.number_system, specs.upper_case));
    copy_num /= specs.number_system;
  }
  if ((specs.accuracy - i > 0 && specs.accuracy != 0 &&
       (!specs.minus || specs.accuracy > specs.width)) ||
      (size_to_num - specs.flag_to_size > 0 && specs.accuracy &&
       (!specs.minus || specs.accuracy > specs.width))) {
    specs.accuracy -= i;
    specs.zero = 1;
  } else
    flag = 1;
  if (size_to_num == 1 && specs.zero == 1 && specs.flag_to_size == 1)
    specs.zero = 0;
  if (specs.accuracy < 0) specs.zero = 0;
  if (num == 0) specs.flag_to_size = 0;
  size_to_num = add_zero_to_str(specs, buffer, size_to_num, &i, flag);
  if (specs.hash && specs.number_system == 8) {
    size_to_num -= add_sym_to_str(buffer + i, &i, '0');
  } else if (specs.hash && specs.number_system == 16 && specs.upper_case) {
    size_to_num -= add_sym_to_str(buffer + i, &i, 'X');
    size_to_num -= add_sym_to_str(buffer + i, &i, '0');
  } else if (specs.hash && specs.number_system == 16 && !specs.upper_case) {
    size_to_num -= add_sym_to_str(buffer + i, &i, 'x');
    size_to_num -= add_sym_to_str(buffer + i, &i, '0');
  }
  if (size_to_num > 0 && specs.minus == 0) {
    while ((size_to_num - specs.flag_to_size > 0) && buffer) {
      size_to_num -= add_sym_to_str(buffer + i, &i, ' ');
    }
  }
  return i;
}

char get_num_char(int num, int upper_case) {
  char flag = '0';
  switch (num) {
    case 10:
      flag = (char)('a' - upper_case * 32);
      break;
    case 11:
      flag = (char)('b' - upper_case * 32);
      break;
    case 12:
      flag = (char)('c' - upper_case * 32);
      break;
    case 13:
      flag = (char)('d' - upper_case * 32);
      break;
    case 14:
      flag = (char)('e' - upper_case * 32);
      break;
    case 15:
      flag = (char)('f' - upper_case * 32);
      break;
  }
  if (0 <= num && num <= 9) flag = (char)(num + 48);
  return flag;
}

char *print_c(char *str, Spec specs, int symbol) {
  char *ptr = s21_NULL;
  int i = 0;
  while (specs.width - 1 > 0 && !specs.minus) {
    str += add_sym_to_str(str, &i, ' ');
    specs.width--;
  }
  if (symbol <= 127) {
    str += add_sym_to_str(str, &i, symbol);
    while (specs.width - 1 > 0 && specs.minus) {
      str += add_sym_to_str(str, &i, ' ');
      specs.width--;
    }
    ptr = str;
  }
  return ptr;
}

char *print_s(char *str, Spec specs, va_list *arguments) {
  char *ptr = str;
  char *string = va_arg(*arguments, char *);
  if (string) {
    int tmp = specs.width, i = 0;
    if ((s21_size_t)specs.width < s21_strlen(string))
      specs.width = s21_strlen(string);
    int blank = specs.width - s21_strlen(string);
    if (specs.accuracy == 0) specs.accuracy = specs.width;
    if (specs.accuracy != 0 && specs.accuracy < tmp)
      blank = tmp - specs.accuracy;
    while (blank && !specs.minus) {
      str += add_sym_to_str(str, &i, ' ');
      blank--;
    }
    while (*string != '\0') {
      if (!specs.accuracy) break;
      str += add_sym_to_str(str, &i, *string);
      string++;
      specs.accuracy--;
    }

    while (blank && specs.minus) {
      str += add_sym_to_str(str, &i, ' ');
      blank--;
    }
  } else {
    str = s21_memcpy(str, "(null)", 6);
    str += 6;
  }
  if (ptr) ptr = str;
  return ptr;
}

char *print_p(char *str, Spec *specs, va_list *arguments) {
  unsigned long int ptr =
      (unsigned long int)va_arg(*arguments, unsigned long int);
  specs->number_system = 16;
  specs->hash = 1;
  specs->upper_case = 0;
  s21_size_t size_to_num = get_buf_size_unsigned_decimal(*specs, ptr);
  char *buffer = malloc(sizeof(char) * size_to_num);
  if (buffer) {
    int i = unsigned_decimal_to_string(buffer, *specs, ptr, size_to_num);
    str += add_buffer_to_string(specs->width, &i, buffer, str);
  }
  if (buffer) free(buffer);
  return str;
}

Spec set_num_sys_double(Spec specs, char format) {
  if (format == 'g' || format == 'G')
    specs.g = 1;
  else if (format == 'e' || format == 'E')
    specs.e = 1;
  if (format == 'G' || format == 'E' || format == 'F') specs.upper_case = 1;
  return specs;
}

s21_size_t get_size_to_double(Spec *specs, long double num) {
  s21_size_t size = 0;
  if (specs->accuracy == 0 && !specs->g && !specs->dot) specs->accuracy = 6;
  long double integer = 0;
  modfl(num, &integer);
  size = specs->accuracy + 1;
  if ((long)integer == 0) size++;
  if (integer < 0) integer = -integer;
  while (((long)integer) > 0) {
    integer /= 10;
    size++;
  }
  if (!specs->accuracy && specs->dot) size--;
  if (specs->space || specs->plus || num < 0) {
    specs->flag_to_size += 1;
    size++;
  }
  if ((s21_size_t)specs->width > size && !specs->e)
    size = specs->width;
  else if ((s21_size_t)specs->width - 4 > size && specs->width - 4 > 0)
    size = specs->width - 4;
  return size;
}

long double normalize(long double *num, Spec *specs) {
  int i = 0;
  if (fabsl(*num) > 1) {
    while (fabsl(*num) > 10) {
      *num /= 10;
      i++;
      specs->e = 2;
    }
  } else {
    while (fabsl(*num) < 0.999999) {
      if (*num == 0) {
        specs->e = 2;
        break;
      }
      *num *= 10;
      i++;
      specs->e = 1;
    }
  }
  return i;
}

Spec cutter(Spec specs, long double num) {
  Spec copy_specs = specs;
  long double copy_num = num;
  int e = normalize(&copy_num, &copy_specs);
  if (copy_specs.accuracy == 0 && !copy_specs.dot) copy_specs.accuracy = 6;
  if ((e <= 4 && copy_specs.e == 1) || (copy_specs.e == 2 && e < 6)) {
    copy_specs.e = 0;
  }
  return copy_specs;
}

s21_size_t is_nan_or_inf(char *str, long double num, Spec specs) {
  int flag = 0;
  if (isnan(num)) {
    if (specs.upper_case) {
      s21_strncpy(str, "NAN", 3);
    } else {
      s21_strncpy(str, "nan", 3);
    }
    flag = 3;
  } else if (num == INFINITY) {
    if (specs.upper_case) {
      s21_strncpy(str, "FNI", 3);
    } else {
      s21_strncpy(str, "fni", 3);
    }
    flag = 3;
  } else if (num == -INFINITY) {
    if (specs.upper_case) {
      s21_strncpy(str, "FNI-", 4);
    } else {
      s21_strncpy(str, "fni-", 4);
    }
    flag = 4;
  }
  return flag;
}

int print_e(int e, s21_size_t *size_to_double, char *str_to_double, Spec specs,
            int *i) {
  int copy_e = e;
  if (copy_e == 0)
    *size_to_double -= add_sym_to_str(
        str_to_double + *i, i,
        get_num_char(copy_e % specs.number_system, specs.upper_case));
  while (copy_e) {
    *size_to_double -= add_sym_to_str(
        str_to_double + *i, i,
        get_num_char(copy_e % specs.number_system, specs.upper_case));
    copy_e /= 10;
  }
  if (e < 10) *size_to_double -= add_sym_to_str(str_to_double + *i, i, '0');
  if (specs.e == 2 || e == 0)
    *size_to_double -= add_sym_to_str(str_to_double + *i, i, '+');
  else
    *size_to_double -= add_sym_to_str(str_to_double + *i, i, '-');
  if (specs.upper_case)
    *size_to_double -= add_sym_to_str(str_to_double + *i, i, 'E');
  else
    *size_to_double -= add_sym_to_str(str_to_double + *i, i, 'e');
  return 0;
}

int add_sym_from_double_to_str(char *str_to_double, Spec specs, int accuracy,
                               int flag_to_dot, s21_size_t size_to_double,
                               int *i, long double frac, long double integer) {
  while (str_to_double && (accuracy != specs.accuracy) && specs.accuracy > 0) {
    if (specs.g && flag_to_dot) {
      size_to_double--;
      break;
    }
    size_to_double -= add_sym_to_str(
        str_to_double + *i, i,
        get_num_char((long)frac % specs.number_system, specs.upper_case));
    frac /= specs.number_system;
    accuracy++;
  }
  if (specs.accuracy) {
    if (flag_to_dot && specs.g) {
      size_to_double--;
    } else
      size_to_double -= add_sym_to_str(str_to_double + *i, i, '.');
  }
  if ((long)integer == 0) {
    size_to_double -= add_sym_to_str(
        str_to_double + *i, i,
        get_num_char((long)integer % specs.number_system, specs.upper_case));
    integer /= specs.number_system;
  }
  while ((long)integer && str_to_double && size_to_double) {
    size_to_double -= add_sym_to_str(
        str_to_double + *i, i,
        get_num_char((long)integer % specs.number_system, specs.upper_case));
    integer /= specs.number_system;
  }
  return size_to_double;
}

int double_to_string(char *str_to_double, Spec specs, long double num,
                     s21_size_t size_to_double, int e) {
  int i = is_nan_or_inf(str_to_double, num, specs);
  if (!i) {
    if (specs.e) print_e(e, &size_to_double, str_to_double, specs, &i);
    long int num_int = 0;
    int flag = invert_num(&num_int, &num);
    long double integer = 0;
    long double frac = modfl(num, &integer);
    int accuracy = specs.accuracy;
    if (frac + 0.000000000000001 >= 1) {
      frac = 0;
      integer += 1;
    }
    long double copy_frac = frac;
    for (int i = 0; i < 15; i++) {
      copy_frac *= 10;
      if ((long)copy_frac != 0) break;
    }
    if ((long)copy_frac == 0 && specs.g) accuracy = 0;
    if (!accuracy && frac * 10 > 4) integer += 1;
    while (accuracy && accuracy > 0) {
      frac *= 10;
      accuracy--;
    }
    if (fmodl(fabsl(frac) * 10, 10.0) > 4) frac = roundl(frac);
    if (specs.g) {
      while ((long)frac % 10 == 0 && specs.accuracy > 0 && (long)frac != 0) {
        frac /= 10;
        accuracy++;
      }
    }
    int flag_to_dot = 0;
    if ((long)frac == 0) flag_to_dot = 1;
    size_to_double =
        add_sym_from_double_to_str(str_to_double, specs, accuracy, flag_to_dot,
                                   size_to_double, &i, frac, integer);
    if (flag) num = -num;
    size_to_double =
        add_zero_to_str(specs, str_to_double, size_to_double, &i, 0);
    long int size = size_to_double;
    size_to_double = add_space_plus_minus(specs, num, &i, str_to_double,
                                          &specs.flag_to_size, &size);
    if (size_to_double > 0 && specs.minus == 0) {
      while ((size_to_double - specs.flag_to_size > 0) && str_to_double) {
        if (specs.g && specs.width < specs.accuracy) break;
        size_to_double -= add_sym_to_str(str_to_double + i, &i, ' ');
      }
    }
  }
  return i;
}

char *print_double(char *str, Spec specs, char format, va_list *arguments) {
  long double num = 0;
  int e = 0;
  if (format == 'L') {
    num = va_arg(*arguments, long double);
  } else
    num = va_arg(*arguments, double);
  s21_size_t size_to_double = get_size_to_double(&specs, num);
  char *str_to_double = malloc(sizeof(char) * size_to_double);
  // for(s21_size_t i = 0; i < size_to_double; i++) str_to_double[i] = 0;
  if (str_to_double) {
    int i = double_to_string(str_to_double, specs, num, size_to_double, e);
    str += add_buffer_to_string(specs.width, &i, str_to_double, str);
  }
  if (str_to_double) free(str_to_double);
  return str;
}

s21_size_t get_size_to_e_g(Spec *specs, long double num) {
  int size = 0;
  int flag = 0;
  if (specs->accuracy == 0 && !specs->g && !specs->dot) {
    specs->accuracy = 6;
  } else if (specs->accuracy == 0 && specs->g) {
    specs->accuracy = 6;
    flag = 1;
  }
  if (specs->g) {
    long double int_part = 0;
    long double float_part = modfl(num, &int_part);
    int count = 1;
    if ((long)int_part > 0) size--;
    while ((long)int_part > 0 && fabsl(num) > 0) {
      int_part /= 10;
      count++;
    }
    specs->accuracy -= count;
    size += count + 1;
    int acc = specs->accuracy;
    if (count > 6) {
      acc = 0;
      size -= 1;
    }
    while (acc && acc > 0 && fabsl(num) > 0) {
      float_part *= 10;
      acc--;
      if ((long)float_part == 0) {
        acc++;
        specs->accuracy++;
      }
      if (specs->dot && flag && (long)float_part != 0) {
        acc++;
        break;
      }
    }
    float_part += 0.0000000001;
    while ((long)float_part % 10 == 0 && specs->accuracy > 0 &&
           fabsl(num) > 0) {
      acc++;
      float_part /= 10;
      size--;
    }
    if (size < 0) size = 0;
    specs->accuracy -= acc - 1;
  }
  if (specs->e) {
    size += specs->accuracy + 1;
    if (specs->accuracy) size++;
    if (specs->e) size += 2;
    if (specs->plus || specs->space || num < 0) size++;
  } else {
    size += specs->accuracy;
  }
  return size;
}

char *print_e_g(char *str, Spec specs, char format, va_list *arguments) {
  long double num = 0;
  int e = 0;
  if (format == 'L') {
    num = va_arg(*arguments, long double);
  } else
    num = va_arg(*arguments, double);
  s21_size_t size = 0;
  if (specs.g) {
    specs = cutter(specs, num);
  }
  if (specs.e) {
    e = normalize(&num, &specs);
    if (e < 100)
      size += 2;
    else
      size += 3;
  }
  size += get_size_to_e_g(&specs, num);
  if ((int)size < specs.accuracy) size = specs.accuracy;
  if ((int)size < specs.width) size = specs.width;
  char *str_to_double = malloc(sizeof(char) * size);
  if (str_to_double) {
    int i = double_to_string(str_to_double, specs, num, size, e);
    str += add_buffer_to_string(specs.width, &i, str_to_double, str);
  }
  if (str_to_double) free(str_to_double);

  return str;
}

char *parser(char *str, char *src, const char *format, Spec specs,
             va_list *arguments) {
  if (*format == 'd' || *format == 'i') {
    str = print_decimal(str, specs, arguments);
  } else if (*format == 'u' || *format == 'o' || *format == 'x' ||
             *format == 'X') {
    specs = set_number_system(specs, *format);
    str = print_u(str, specs, *(format - 1), arguments);
  } else if (*format == 'c') {
    int symbol = va_arg(*arguments, int);
    str = print_c(str, specs, symbol);
  } else if (*format == 's') {
    str = print_s(str, specs, arguments);
  } else if (*format == 'p') {
    str = print_p(str, &specs, arguments);
  } else if (*format == 'n') {
    int *n = va_arg(*arguments, int *);
    *n = (int)(str - src);
  } else if (*format == 'f' || *format == 'F') {
    specs = set_num_sys_double(specs, *format);
    str = print_double(str, specs, *(format - 1), arguments);
  } else if (*format == 'e' || *format == 'E' || *format == 'g' ||
             *format == 'G') {
    specs = set_num_sys_double(specs, *format);
    str = print_e_g(str, specs, *(format - 1), arguments);
  } else if (*format == '%') {
    str = print_c(str, specs, '%');
  } else {
    str = s21_NULL;
  }
  if (!str) *src = '\0';
  return str;
}