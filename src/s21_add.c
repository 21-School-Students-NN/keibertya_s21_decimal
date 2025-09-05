#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

/**
 * @brief Static helper function to emulate _addcarry_u32 intrinsic
 * @param carry Input carry flag (0 or 1)
 * @param x First 32-bit addend
 * @param y Second 32-bit addend
 * @param result Pointer to store the 32-bit sum
 *
 * @return Output carry flag (0 or 1)
 */
static int _add_with_carry(int carry, const s21_decimal* x,
                           const s21_decimal* y, s21_decimal* result) {
  for (int i = 0; i < 3; ++i) {
    uint64_t sum = (uint64_t)x->bits[i] + y->bits[i] + (carry ? 1 : 0);
    result->bits[i] = (uint32_t)sum;
    carry = (sum >> 32);
  }

  return carry;
}

/**
 * @brief Multiply by collomn with coverage data
 * @param carry_in Number to carry
 * @param x first addendum
 * @param y second addendum
 * @param result pointer to store the result
 * @return int32_t carry data - does the myltiply overflow
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 15.08.2025
 */
int32_t _multiply_collomn(uint32_t carry_in, uint32_t x, uint32_t y,
                          uint32_t* result) {
  uint64_t product = (uint64_t)x * y + carry_in;
  *result = (uint32_t)product;
  return product >> 32;
}

int normalizing(s21_decimal* value_1, s21_decimal* value_2) {
  if (_get_scale(value_1) == _get_scale(value_2)) return S21_SUCCESS;

  s21_decimal *ptr_low_scale, *ptr_high_scale;

  s21_decimal tmp_value_1 = *value_1;
  s21_decimal tmp_value_2 = *value_2;

  // find decimal withh lowest and highest scale
  int order = (_get_scale(value_1) < _get_scale(value_2));
  if (order) {
    ptr_low_scale = &tmp_value_1;
    ptr_high_scale = &tmp_value_2;
  } else {
    ptr_low_scale = &tmp_value_2;
    ptr_high_scale = &tmp_value_1;
  }

  int resp = S21_SUCCESS;

  while ((_get_scale(ptr_low_scale) != _get_scale(ptr_high_scale)) && !resp) {
    if (_normalize_to_upper(ptr_low_scale)) {
      if (_get_scale(ptr_high_scale) == 0) {
        resp = _get_sign(ptr_low_scale) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      // all things around that fnction should be plased inside of it
      divide_and_round(ptr_high_scale, 0);
      _set_scale(ptr_high_scale, _get_scale(ptr_high_scale) - 1);
    }
  }

  // change the values to normolized saving order
  if (!resp) {
    if (order) {
      *value_1 = *ptr_low_scale;
      *value_2 = *ptr_high_scale;
    } else {
      *value_2 = *ptr_low_scale;
      *value_1 = *ptr_high_scale;
    }
  }

  return resp;
}

static int _normalize_to_upper(s21_decimal* to_normalize) {
  uint32_t carry = 0;
  s21_decimal tmp = *to_normalize;

  for (int i = 0; i < 3; ++i)
    carry = _multiply_collomn(carry, tmp.bits[i], 10, &tmp.bits[i]);

  if (!carry) {
    *to_normalize = tmp;
    _set_scale(to_normalize, _get_scale(to_normalize) + 1);
  }

  return carry ? S21_ERROR : S21_SUCCESS;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return S21_ERROR;
  _init_decimal_zero(result);

  // redirect to s21_sub in case of different sign
  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    s21_decimal *minuend, *subtrahend;
    if (_get_sign(&value_1) > _get_sign(&value_2)) {
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
  if (_is_decimal_zero(&value_1) || _is_decimal_zero(&value_2)) {
    if (_is_decimal_zero(&value_1))
      *result = value_2;
    else
      *result = value_1;
    // if not zero, trying to normalize
  } else if (!(response = normalizing(&value_1, &value_2))) {
    s21_decimal tmp_result = {0};

    uint32_t carry = _add_with_carry(0, &value_1, &value_2, &tmp_result);
    int final_scale = _get_scale(&value_1);

    while (carry > 0 && final_scale--)
      carry = divide_and_round(&tmp_result, carry);

    if (carry) {
      response = _get_sign(&value_1) ? S21_TOO_SMALL : S21_TOO_LARGE;
    } else {
      for (int i = 0; i < 3; ++i) result->bits[i] = tmp_result.bits[i];
      _set_sign(result, _get_sign(&value_1));
      _set_scale(result, final_scale);
    }
  }

  return response;
}
