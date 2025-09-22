#include <math.h>
#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

#define MAX_DEC_VALUE 7.9228162514264337593543950335E+28

static const s21_decimal max_int_dec = {{0x7FFFFFFF, 0, 0, 0}};
static const s21_decimal min_int_dec = {{(int)0x80000000, 0, 0, 0x80000000}};

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = S21_ERROR;
  if (dst) {
    _init_decimal_zero(dst);
    if (src >= 0) {        // positive integer
      dst->bits[0] = src;  // write int into bits[0]
      result = S21_SUCCESS;
    } else if (_set_sign(dst, 1) == S21_SUCCESS) {
      // negative integer
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
  int result = S21_ERROR;
  if (dst) {
    _init_decimal_zero(dst);
    float abs_src = fabs(src);
    if (abs_src > 1e-28 && abs_src < MAX_DEC_VALUE) {
      char number[20];
      sprintf(number, "%.6E", abs_src);
      int int_val = 0;
      char *ptr = number;
      for (int i = 1000000; i >= 1; i /= 10) {
        if (*ptr == '.') ptr++;
        int_val += (*ptr - '0') * i;
        ptr++;
      }
      dst->bits[0] = int_val;
      short exp = strtol(ptr + 1, NULL, 10) - 6;
      if (exp > 0) {
        for (int e = 0; e < exp; ++e) {
          _multiply_by_10(dst);
        }
      } else if (exp < -28) {
        for (int e = exp; e < -28; ++e) {
          dst->bits[0] /= 10;
        }
        dst->bits[3] = (int)(28) << 16;
      } else {
        dst->bits[3] = (int)(-exp) << 16;
      }
      if (src < 0) _set_sign(dst, 1);
      result = S21_SUCCESS;
    }
  }
  return result;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = S21_ERROR;
  if (dst && s21_is_less_or_equal(src, max_int_dec) &&
      s21_is_greater_or_equal(src, min_int_dec)) {
    *dst = 0;
    if (s21_truncate(src, &src) == S21_SUCCESS) {
      int scale = _get_scale(&src);
      if (scale) {
        for (int e = 0; e < scale; ++e) {
          _divide_by_10(&src, 0);
        }
      }
      *dst = (_get_sign(&src) ? -1 : 1) * src.bits[0];
      result = S21_SUCCESS;
    }
  }
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = S21_ERROR;
  if (dst) {
    *dst = 0.0;
    for (unsigned i = 0; i < 96; ++i) {
      meta_t bit = _get_bit(&src, i);
      if (bit) *dst += (float)pow(2, i);
    }
    int scale = _get_scale(&src);
    *dst *= (_get_sign(&src) ? -1 : 1) * pow(0.1, scale);
    result = S21_SUCCESS;
  }
  return result;
}
