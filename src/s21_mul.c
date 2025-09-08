#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return S21_ERROR;

  _init_decimal_zero(result);

  int status_code = S21_SUCCESS;

  if (_is_decimal_zero(&value_1) || _is_decimal_zero(&value_2))
    return status_code;

  uint64_t product[6] = {0};  // Max result: 96 + 96 = 192 bit (6 words)

  for (int i = 0; i < 3; ++i) {
    uint64_t carry = 0;
    for (int j = 0; j < 3; ++j) {
      carry += (uint64_t)value_1.bits[i] * value_2.bits[j];

      product[i + j] += carry & 0xFFFFFFFF;
      carry = carry >> 32;
    }
    product[i + 3] += carry;
  }

  // again normalize the value inside product parts
  for (int i = 0; i < 5; i++) {
    product[i + 1] += product[i] >> 32;
    product[i] &= 0xFFFFFFFF;
  }

  int result_sign = _get_sign(&value_1) ^ _get_sign(&value_2);
  int result_scale = _get_scale(&value_1) + _get_scale(&value_2);

  if (!(product[3] || product[4] || product[5]) && result_scale <= MAX_SCALE) {
    for (int i = 0; i < 3; ++i) result->bits[i] = product[i];

    _set_sign(result, result_sign);
    _set_scale(result, result_scale);
  } else {
    status_code =
        _normalize_product(product, result_scale, result_sign, result);
  }

  return status_code;
}