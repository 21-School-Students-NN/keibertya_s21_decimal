#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int _is_decimal_zero(s21_decimal* num) {
  // return `1` if there is NO digit in mantisa
  return !(num->bits[0] | num->bits[1] | num->bits[2]);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (_is_decimal_zero(&value_2)) return S21_DIV_BY_ZERO;
  _init_decimal_zero(result);
  // Placeholder minimal behavior: sign and scale handling skeleton
  meta_t sign = (_get_sign(&value_1) ^ _get_sign(&value_2));
  meta_t scale = 0;  // to be calculated via normalization later
  (void)scale;
  _set_sign(result, sign);
  return S21_ERROR;  // not yet implemented
}
