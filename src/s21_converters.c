#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = S21_ERROR;
  if (dst) {
    _init_decimal_zero(dst);
    if (src >= 0) {
      dst->bits[0] = src;
      result = S21_SUCCESS;
    } else if (_set_sign(dst, 1) == S21_SUCCESS) {
      if (src != 0x80000000)
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
  _init_decimal_zero(dst);
  if (dst) {
    char number[16];
    if (sprintf(number, "%.6e", src) == 1) {
      result = S21_SUCCESS;
    }
  }
  return result;
}