#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return S21_ERROR;
  }

  if (_is_decimal_zero(&value_2)) {
    return S21_DIV_BY_ZERO;
  }

  _init_decimal_zero(result);

  if (_is_decimal_zero(&value_1)) {
    // 0 / anything = 0
    return S21_SUCCESS;
  }

  int sign1 = _get_sign(&value_1);
  int sign2 = _get_sign(&value_2);
  int result_sign = sign1 ^ sign2;

  s21_uint192_t dividend = {{0, 0, 0, 0, 0, 0}};
  s21_uint192_t divisor = {{0, 0, 0, 0, 0, 0}};
  s21_uint192_t quotient = {{0, 0, 0, 0, 0, 0}};
  s21_uint192_t remainder = {{0, 0, 0, 0, 0, 0}};

  from_decimal_to_int192(value_1, &dividend);
  from_decimal_to_int192(value_2, &divisor);

  int scale1 = _get_scale(&value_1);
  int scale2 = _get_scale(&value_2);
  int result_scale = scale1 - scale2;
  if (result_scale < 0)
    for (; result_scale < 0; ++result_scale) uint192_mult_by_10(&dividend);
  else if (result_scale > 0)
    for (; result_scale > 0; --result_scale) uint192_mult_by_10(&divisor);

  uint192_div(dividend, divisor, &quotient, &remainder);

  if (quotient.bits[3] | quotient.bits[4] | quotient.bits[5])
    return result_sign ? S21_TOO_SMALL : S21_TOO_LARGE;

  if (uint192_is_not_zero(&remainder)) {
    // add decimal digits while scale < max_scale
    while (uint192_is_not_zero(&remainder) && result_scale < MAX_PRECISION &&
           quotient.bits[5] < 0x204FCE5E) {
      uint192_mult_by_10(&remainder);
      uint192_mult_by_10(&quotient);
      result_scale++;
      if (uint192_compare(remainder, divisor) >= 1) {
        s21_uint192_t new_quotient = {{0, 0, 0, 0, 0, 0}};
        s21_uint192_t new_remainder = {{0, 0, 0, 0, 0, 0}};

        uint192_div(remainder, divisor, &new_quotient, &new_remainder);

        uint192_add(quotient, new_quotient, &quotient);
        remainder = new_remainder;
      }
    }
  }
  int code = from_uint192_to_decimal(&quotient, result_scale, result);
  _set_sign(result, result_sign);
  if (code == S21_TOO_LARGE && result_sign) code = S21_TOO_SMALL;

  return code;
}