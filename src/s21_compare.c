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

  s21_sub(value_1, value_2, &value_1);

  return _is_decimal_zero(&value_1);
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;

  if (s21_is_equal(value_1, value_2)) {
    return S21_FALSE;
  }

  s21_sub(value_1, value_2, &value_1);
  if (_get_sign(&value_1) == 1) {
    is_less = 1;
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