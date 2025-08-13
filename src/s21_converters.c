#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

#define MAX_DEC_VALUE 7.9228162514264337593543950335E+28

static const s21_decimal ten = {0, 0, 0, 10};

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = S21_ERROR;
  if (dst) {
    _init_decimal_zero(dst);
    if (src >= 0) {
      dst->bits[0] = src;
      result = S21_SUCCESS;
    } else if (_set_sign(dst, 1) == S21_SUCCESS) {
      if (src != (int)0x80000000)
        dst->bits[0] = -src;
      else
        dst->bits[0] = src;
      result = S21_SUCCESS;
    }
  }
  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = S21_SUCCESS;
  _init_decimal_zero(dst);
  if (dst) {
    float abs_src = fabs(src);
    if (((abs_src > 0 && abs_src < 1e-28)) || abs_src > MAX_DEC_VALUE) {
      result = S21_ERROR;
    } else {
      char number[20];
      if (sprintf(number, "%.6E", abs_src) == 1) {
        int int_val = 0;
        char *ptr = number;
        for (int i = 1000000; i != 0; i /= 10) {
          if (*ptr == '.') ptr++;
          int_val += (*ptr - '0') * i;
          ptr++;
        }
        s21_from_int_to_decimal(int_val, dst);
        short exp = strtol(ptr + 1, NULL, 10) - 6;
        if (exp > 0) {
          while (exp > 0) {
            s21_mul(*dst, ten, dst);
            exp--;
          }
        } else if (exp < -28) {
          dst->bits[3] = (int)(28) << 16;
          while (exp < 28) {
            s21_div(*dst, ten, dst);
            exp++;
          }
        } else {
          dst->bits[3] = (int)(-exp) << 16;
        }
        if (src < 0) result = _set_sign(dst, 1);
      }
    }
  }
  return result;
}