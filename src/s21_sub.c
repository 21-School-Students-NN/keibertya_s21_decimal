#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return S21_ERROR;

  _init_decimal_zero(result);

  if (_is_decimal_zero(&value_2)) {
    for (int i = 0; i < 4; ++i) result->bits[i] = value_1.bits[i];
    return S21_SUCCESS;
  }
  if (_is_decimal_zero(&value_1)) {
    for (int i = 0; i < 4; ++i) result->bits[i] = value_2.bits[i];
    _set_sign(result, 1 - _get_sign(&value_2));
    return S21_SUCCESS;
  }

  // A - B  <=>  A + (-B)
  if (_get_sign(&value_1) == !_get_sign(&value_2)) {
    _set_sign(&value_2, _get_sign(&value_1));
    return s21_add(value_1, value_2, result);
  }

  s21_uint192_t res1;
  s21_uint192_t res2;
  from_decimal_to_int192(value_1, &res1);
  from_decimal_to_int192(value_2, &res2);
  meta_t scale = leveling(value_1, value_2, &res1, &res2);
  int sign_flag = uint192_sub(res1, res2, &res1);
  int response = from_uint192_to_decimal(&res1, scale, result);
  _set_sign(result,
            sign_flag ? _get_sign(&value_1) : (1 - _get_sign(&value_1)));
  if (response == S21_TOO_LARGE && _get_sign(result)) response = S21_TOO_SMALL;

  return response;
}