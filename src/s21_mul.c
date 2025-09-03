#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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

  if (product[3] || product[4] || product[5]) {
    return _get_sign(result) ? S21_TOO_SMALL : S21_TOO_LARGE;
  }

  // Save the answer to `result`
  for (int i = 0; i < 3; ++i) result->bits[i] = product[i];

  // XOR the sign for product
  _set_sign(result, (_get_sign(&value_1) ^ _get_sign(&value_2)));

  // use banking rounding if scale > 28
  _set_scale(result, _get_scale(&value_1) + _get_scale(&value_2));

  return S21_SUCCESS;
}