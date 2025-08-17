#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

/**
 * @brief Static helper function to emulate _addcarry_u32 intrinsic
 * @param carry_in Input carry flag (0 or 1)
 * @param x First 32-bit addend
 * @param y Second 32-bit addend
 * @param result Pointer to store the 32-bit sum
 *
 * @return Output carry flag (0 or 1)
 */
static inline meta_t _add_with_carry(meta_t carry_in, uint32_t x, uint32_t y,
                                     uint32_t* result) {
  uint64_t sum = (uint64_t)x + y + (carry_in ? 1 : 0);
  *result = (uint32_t)sum;
  return (sum >> 32) & 1;
}

static inline int32_t _multiply_collomn(uint32_t carry_in, uint32_t x,
                                        uint32_t y, uint32_t* result) {
  uint64_t product = (uint64_t)x * y + carry_in;
  *result = (uint32_t)product;
  return product >> 32;
}

#if 0
static int _add_normalized(uint32_t carry_in, s21_decimal *x, s21_decimal *y, s21_decimal *result) {
  for (int i = 0; i < 3; ++i)
    carry_in =
        _add_with_carry(carry_in, x->bits[i], y->bits[i], &result->bits[i]);
  return carry_in;
}
#endif

static int _normalize_to_upper(s21_decimal* to_normalize, meta_t diff) {
  uint32_t carry = 0;
  s21_decimal tmp = *to_normalize;

  for (int d = 0; d < diff && !carry; ++d)
    for (int i = 0; i < 3; ++i)
      carry = _multiply_collomn(carry, tmp.bits[i], 10, &tmp.bits[i]);

  if (!carry) *to_normalize = tmp;

  return carry ? S21_ERROR : S21_SUCCESS;
}

static void _swap(meta_t* a, meta_t* b) {
  if (a != b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  _init_decimal_zero(result);

  meta_t max_scale = _get_scale(&value_1);
  meta_t min_scale = _get_scale(&value_2);
  if (max_scale < min_scale) _swap(&max_scale, &min_scale);

  meta_t diff = max_scale - min_scale;

  uint32_t carry = 0;
#if 0  // wait for `s21_sub` implemantation
  if (_get_sign(&value_1) != _get_sign(&value_2))
    s21_sub(value_1, value_2, result);
#endif

  // try to normolize | handle `diff == 0` into `_normalize_to_upper()`
  if (_normalize_to_upper(
          (min_scale == _get_scale(&value_1)) ? &value_1 : &value_2, diff)) {
    ;  // place for banking rounding
  } else {
    for (int i = 0; i < 3; ++i)
      carry = _add_with_carry(carry, value_1.bits[i], value_2.bits[i],
                              &result->bits[i]);

    _set_sign(result, _get_sign(&value_1));
    _set_scale(result, max_scale);
  }

  // TODO(trelawnm): find more convenient way to return
  if (carry) {
    return _get_sign(result) ? S21_TOO_SMALL : S21_TOO_LARGE;
  }
  return 0;
}
