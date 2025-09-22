#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return S21_ERROR;
  _init_decimal_zero(result);

  // redirect to s21_sub in case of different sign
  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    const s21_decimal *minuend, *subtrahend;
    if (_get_sign(&value_1)) {  // negative value_1
      _set_sign(&value_1, 0);
      minuend = &value_2;
      subtrahend = &value_1;
    } else {
      _set_sign(&value_2, 0);
      minuend = &value_1;
      subtrahend = &value_2;
    }
    return s21_sub(*minuend, *subtrahend, result);
  }

  // handle case where one of dec is zero
  int response = S21_SUCCESS;
  /*if (_is_decimal_zero(&value_1) || _is_decimal_zero(&value_2)) {
    if (_is_decimal_zero(&value_1))
      *result = value_2;
    else
      *result = value_1;
    // if not zero, trying to normalize
  } else if (!(response = _normalize(&value_1, &value_2))) {
    s21_decimal tmp_result = {0};

    uint32_t carry = _add_with_carry(&value_1, &value_2, &tmp_result);
    int final_scale = _get_scale(&value_1);

    while (carry > 0 && final_scale--)
      carry = _divide_and_round(&tmp_result, carry);

    if (carry) {
      response = _get_sign(&value_1) ? S21_TOO_SMALL : S21_TOO_LARGE;
    } else {
      for (int i = 0; i < 3; ++i) result->bits[i] = tmp_result.bits[i];
      _set_sign(result, _get_sign(&value_1));
      _set_scale(result, final_scale);
    }
  }*/

  if (_is_decimal_zero(&value_1) || _is_decimal_zero(&value_2)) {
    if (_is_decimal_zero(&value_1))
      *result = value_2;
    else
      *result = value_1;
    // if not zero, trying to normalize
  } else {
    s21_uint192_t res1;
    s21_uint192_t res2;
    from_decimal_to_int192(value_1, &res1);
    from_decimal_to_int192(value_2, &res2);
    meta_t scale = leveling(value_1, value_2, &res1, &res2);
    uint192_add(res1, res2, &res1);
    response = from_uint192_to_decimal(&res1, scale, result);
    if (response == S21_TOO_LARGE && _get_sign(&value_1))
      response = S21_TOO_SMALL;
    _set_sign(result, _get_sign(&value_1));
  }

  return response;
}
