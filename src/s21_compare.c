#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  s21_sub(value_1, value_2, &value_1);
  return _is_decimal_zero(&value_1);
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  s21_sub(value_1, value_2, &value_1);
  return _is_decimal_zero(&value_1) ? S21_FALSE : _get_sign(&value_1);
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