#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _init_decimal_zero(result);
  uint32_t temp192[6] = {0};
  meta_t sign = (_get_sign(&value_1) ^ _get_sign(&value_2));
  meta_t scale = _get_scale(&value_1) + _get_scale(&value_2);

  _multiply_mantissas_96(&value_1, &value_2, temp192);

  if (temp192[3] || temp192[4] || temp192[5] || scale > MAX_SCALE) {
    int norm = _normalize_and_fit_192_to_96(temp192, &scale, sign, result);
    return norm;
  }

  result->bits[0] = (int)temp192[0];
  result->bits[1] = (int)temp192[1];
  result->bits[2] = (int)temp192[2];
  _set_sign(result, sign);
  _set_scale(result, scale);
  return S21_SUCCESS;
}