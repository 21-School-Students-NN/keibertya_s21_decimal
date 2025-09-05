#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (_is_decimal_zero(&value_2)) return S21_DIV_BY_ZERO;
  _init_decimal_zero(result);

  _set_sign(&value_1, 1);

  return S21_SUCCESS;
}
