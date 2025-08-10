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
static inline meta_t _adc_u32(meta_t carry_in, uint32_t x, uint32_t y,
                              uint32_t* result) {
  uint64_t sum = (uint64_t)x + y + (carry_in ? 1 : 0);
  *result = (uint32_t)sum;
  return (sum >> 32) & 1;
}

void _swap(meta_t *a, meta_t *b) {
  if (a != b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
  }
}

// TODO(trelawnm): normilize number with smaller scale to biggest one *10^diff
// of scale and carfully look for overflow, and decrease scale biggest one to
// smaller in that case

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  _init_decimal_zero(result);

  meta_t max_scale = _get_scale(&value_1);
  meta_t min_scale = _get_scale(&value_2);
  if (max_scale < min_scale) _swap(&max_scale, &min_scale);

  meta_t diff = max_scale - min_scale;

  // if scale are same - we can add direclty TODO(trelawnm): add sign check
  meta_t carry = 0;
  if (diff == 0) {
    for (int i = 0; i < 3; ++i)
      carry =
          _adc_u32(carry, value_1.bits[i], value_2.bits[i], &result->bits[i]);

    _set_sign(result, _get_sign(&value_1));

    if (carry) {
      return _get_sign(result) ? 2 : 1;
    }
  } else if (diff + min_scale < MAX_SCALE) {
    // should be refactored by using hand-writen multiply
    _normalize_to_upper((min_scale == _get_scale(&value_1) ? &value_1 : &value_2), diff);

    for (int i = 0; i < 3; ++i)
    carry =
        _adc_u32(carry, value_1.bits[i], value_2.bits[i], &result->bits[i]);

    _set_sign(result, _get_sign(&value_1));

    if (carry) {
      return _get_sign(result) ? 2 : 1;
    }
  }

  // if scale not the same, and we can normilize smaller to biggest - normalize

  // if we can't normilize - we should round with bank-rounding


  // probably can be recursive

  // TODO: handle different sign
  return 0;
}
