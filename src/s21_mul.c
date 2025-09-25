#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return S21_ERROR;

  _init_decimal_zero(result);
  int status_code = S21_SUCCESS;

  if (_is_decimal_zero(&value_1) || _is_decimal_zero(&value_2))
    return status_code;

  int result_sign = _get_sign(&value_1) ^ _get_sign(&value_2);
  int result_scale = _get_scale(&value_1) + _get_scale(&value_2);

  s21_uint192_t product = uint192_mul(value_1, value_2);
  status_code = from_uint192_to_decimal(&product, result_scale, result);
  _set_sign(result, result_sign);
  if (status_code == S21_TOO_LARGE && result_sign) status_code = S21_TOO_SMALL;
  if (_is_decimal_zero(result) && status_code == S21_SUCCESS)
    status_code = S21_TOO_SMALL;
  return status_code;
}