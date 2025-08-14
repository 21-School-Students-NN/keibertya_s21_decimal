#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

#define MAX_DEC_VALUE 7.9228162514264337593543950335E+28

static const s21_decimal max_int_dec = {{0x7FFFFFFF, 0, 0, 0}};
static const s21_decimal min_int_dec = {{(int)0x80000000, 0, 0, 0x80000000}};

static const s21_decimal ten_pows[29] = {
    [0] = {{0x1, 0x0, 0x0, 0x0}},
    [1] = {{0xA, 0x0, 0x0, 0x0}},
    [2] = {{0x64, 0x0, 0x0, 0x0}},
    [3] = {{0x3E8, 0x0, 0x0, 0x0}},
    [4] = {{0x2710, 0x0, 0x0, 0x0}},
    [5] = {{0x186A0, 0x0, 0x0, 0x0}},
    [6] = {{0xF4240, 0x0, 0x0, 0x0}},
    [7] = {{0x989680, 0x0, 0x0, 0x0}},
    [8] = {{0x5F5E100, 0x0, 0x0, 0x0}},
    [9] = {{0x3B9ACA00, 0x0, 0x0, 0x0}},
    [10] = {{0x540BE400, 0x2, 0x0, 0x0}},
    [11] = {{0x4876E800, 0x17, 0x0, 0x0}},
    [12] = {{0xD4A51000, 0xE8, 0x0, 0x0}},
    [13] = {{0x4E72A000, 0x918, 0x0, 0x0}},
    [14] = {{0x107A4000, 0x5AF3, 0x0, 0x0}},
    [15] = {{0xA4C68000, 0x38D7E, 0x0, 0x0}},
    [16] = {{0x6FC10000, 0x2386F2, 0x0, 0x0}},
    [17] = {{0x5D8A0000, 0x1634578, 0x0, 0x0}},
    [18] = {{0xA7640000, 0xDE0B6B3, 0x0, 0x0}},
    [19] = {{0x89E80000, 0x8AC72304, 0x0, 0x0}},
    [20] = {{0x63100000, 0x6BC75E2D, 0x5, 0x0}},
    [21] = {{0xDEA00000, 0x35C9ADC5, 0x36, 0x0}},
    [22] = {{0xB2400000, 0x19E0C9BA, 0x21E, 0x0}},
    [23] = {{0xF6800000, 0x2C7E14A, 0x152D, 0x0}},
    [24] = {{0xA1000000, 0x1BCECCED, 0xD3C2, 0x0}},
    [25] = {{0x4A000000, 0x16140148, 0x84595, 0x0}},
    [26] = {{0xE4000000, 0xDCC80CD2, 0x52B7D2, 0x0}},
    [27] = {{0xE8000000, 0x9FD0803C, 0x33B2E3C, 0x0}},
    [28] = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}}};

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
          s21_mul(*dst, ten_pows[exp], dst);
        } else if (exp < -28) {
          dst->bits[3] = (int)(28) << 16;
          s21_div(*dst, ten_pows[-28 - exp], dst);
        } else {
          dst->bits[3] = (int)(-exp) << 16;
        }
        if (src < 0) result = _set_sign(dst, 1);
      }
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
      if (bit) dst += pow(2, i);
    }
    int scale = _get_scale(&src);
    *dst /= (_get_sign(&src) ? -1 : 1) * pow(10, scale);
    result = S21_SUCCESS;
  }
  return result;
}
