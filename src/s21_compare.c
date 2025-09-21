#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return S21_TRUE;
  }

  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    return S21_FALSE;
  }

  _normalize(&value_1, &value_2);

  return _compare_mantissas(&value_1, &value_2) == 0;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;
  int sign1 = _get_sign(&value_1);
  int sign2 = _get_sign(&value_2);

  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return S21_FALSE;
  }

  if (sign1 != sign2) {
    is_less = (sign1 > sign2);
  } else {
    _normalize(&value_1, &value_2);
    int mantissa_cmp = _compare_mantissas(&value_1, &value_2);

    if (sign1 == 0) {
      if (mantissa_cmp == -1) {
        is_less = 1;
      }
    } else {
      if (mantissa_cmp == 1) {
        is_less = 1;
      }
    }
  }

  return is_less;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_2, value_1);
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_greater(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}