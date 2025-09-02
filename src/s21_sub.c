#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _init_decimal_zero(result);
  // If signs equal -> subtraction; if different -> delegate to add
  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    // a - (-b) == a + b; a - b with different signs becomes addition
    _set_sign(&value_2, _get_sign(&value_1));
    return s21_add(value_1, value_2, result);
  }

  // Normalize scales
  if (_normalize_scales_meet(
          (_get_scale(&value_1) <= _get_scale(&value_2)) ? &value_1 : &value_2,
          (_get_scale(&value_1) > _get_scale(&value_2)) ? &value_1 : &value_2))
    return _get_sign(&value_1) ? S21_TOO_SMALL : S21_TOO_LARGE;

  // Determine which mantissa is bigger in absolute value
  const s21_decimal *big = &value_1;
  const s21_decimal *small = &value_2;
  int cmp = _compare_mantissas_96(&value_1, &value_2);
  if (cmp < 0) {
    big = &value_2;
    small = &value_1;
  }

  // Subtract mantissas with borrow: big - small
  uint64_t borrow = 0;
  for (int i = 0; i < 3; ++i) {
    uint64_t minuend = (uint64_t)(uint32_t)big->bits[i];
    uint64_t subtr = (uint64_t)(uint32_t)small->bits[i] + borrow;
    uint64_t diff = (minuend + (1ull << 32)) - subtr;
    result->bits[i] = (int)(diff & 0xFFFFFFFFu);
    borrow = (subtr > minuend) ? 1 : 0;
  }

  // Set sign: sign equals sign of the greater absolute value
  _set_sign(result, _get_sign(big));
  _set_scale(result, _get_scale(big));

  // If result is zero -> reset sign and scale
  if (!(result->bits[0] | result->bits[1] | result->bits[2])) {
    _set_sign(result, 0);
    _set_scale(result, 0);
  }

  return S21_SUCCESS;
}