#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return S21_ERROR;
  _init_decimal_zero(result);

  if (_is_decimal_zero(&value_2)) {
    for (int i = 0; i < 4; ++i) result->bits[i] = value_1.bits[i];
    return S21_SUCCESS;
  }

  // A - B  <=>  A + (-B)
  if (_get_sign(&value_1) == !_get_sign(&value_2)) {
    _set_sign(&value_2, _get_sign(&value_1));
    return s21_add(value_1, value_2, result);
  }

  s21_decimal norm_val1 = value_1;
  s21_decimal norm_val2 = value_2;

  int norm_error = _normalize(&norm_val1, &norm_val2);
  if (norm_error) return norm_error;

  s21_decimal *ptr_big, *ptr_small;
  int should_be_changed = (_compare_mantissas(&norm_val1, &norm_val2) < 0);
  if (should_be_changed) {
    ptr_big = &norm_val2;
    ptr_small = &norm_val1;
  } else {
    ptr_big = &norm_val1;
    ptr_small = &norm_val2;
  }

  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t diff = (uint64_t)ptr_big->bits[i] - ptr_small->bits[i] - borrow;
    result->bits[i] = (uint32_t)diff;

    borrow = (diff >> 63) & 1;
  }

  _set_sign(result,
            should_be_changed ? 1 - _get_sign(&value_1) : _get_sign(&value_1));
  _set_scale(result, _get_scale(&norm_val1));

  return S21_SUCCESS;
}