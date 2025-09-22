#include "../include/s21_helpers.h"

#include "../include/s21_decimal.h"

//======================================================================
//  Sign & Scale
//======================================================================

meta_t _get_scale(const s21_decimal *dec) {
  return (meta_t)((dec->bits[3] & SCALE_MASK) >> 16);
}

int _set_scale(s21_decimal *dec, const meta_t scale) {
  if (scale > MAX_SCALE) return S21_ERROR;
  dec->bits[3] = (dec->bits[3] & SIGN_MASK) | ((uint32_t)scale << 16);
  return S21_SUCCESS;
}

meta_t _get_sign(const s21_decimal *dec) {
  return (meta_t)((dec->bits[3] & SIGN_MASK) >> 31);
}

int _set_sign(s21_decimal *dec, const meta_t sign) {
  if (sign > MAX_SIGN) return S21_ERROR;
  dec->bits[3] = (dec->bits[3] & SCALE_MASK) | ((uint32_t)sign << 31);
  return S21_SUCCESS;
}

//======================================================================
//  Other
//======================================================================

void _init_decimal_zero(s21_decimal *dec) {
  memset(dec, 0, sizeof(s21_decimal));
}

int _is_equal(s21_decimal value1, s21_decimal value2) {
  return value1.bits[0] == value2.bits[0] && value1.bits[1] == value2.bits[1] &&
         value1.bits[2] == value2.bits[2] && value1.bits[3] == value2.bits[3];
}

void _bank_round(s21_decimal *value, uint32_t remainder, int scale) {
  if (remainder > 5) {
    // Rounding up
    s21_decimal one = {{1, 0, 0, 0}};
    _set_scale(&one, scale);
    _set_sign(&one, _get_sign(value));
    s21_add(*value, one, value);
  } else if (remainder == 5) {
    // Bank rounding: rounding to the nearest even number
    if (value->bits[0] & 1) {
      s21_decimal one = {{1, 0, 0, 0}};
      _set_scale(&one, scale);
      _set_sign(&one, _get_sign(value));
      s21_add(*value, one, value);
    }
  }
}

void _subtract_mantissas(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result) {
  uint64_t diff;
  uint32_t borrow = 0;

  diff = (uint64_t)value_1.bits[0] - value_2.bits[0] - borrow;
  result->bits[0] = (uint32_t)diff;
  borrow = (diff >> 32) ? 1 : 0;

  diff = (uint64_t)value_1.bits[1] - value_2.bits[1] - borrow;
  result->bits[1] = (uint32_t)diff;
  borrow = (diff >> 32) ? 1 : 0;

  diff = (uint64_t)value_1.bits[2] - value_2.bits[2] - borrow;
  result->bits[2] = (uint32_t)diff;
}

int _is_decimal_zero(const s21_decimal *dec) {
  return !(dec->bits[0] | dec->bits[1] | dec->bits[2]);
}

//======================================================================
//  Arithmetic
//======================================================================

int _normalize(s21_decimal *value_1, s21_decimal *value_2) {
  if (_get_scale(value_1) == _get_scale(value_2)) return S21_SUCCESS;
  if (value_1 == value_2) return S21_ERROR;

  // pointers for find lowest and highest scales
  s21_decimal *ptr_low_scale, *ptr_high_scale;

  // temporary value for changing
  s21_decimal tmp_value_1 = *value_1;
  s21_decimal tmp_value_2 = *value_2;

  // find decimal with lowest and highest scale
  int order = (_get_scale(value_1) < _get_scale(value_2));
  if (order) {
    ptr_low_scale = &tmp_value_1;
    ptr_high_scale = &tmp_value_2;
  } else {
    ptr_low_scale = &tmp_value_2;
    ptr_high_scale = &tmp_value_1;
  }

  int status_code = S21_SUCCESS;

  while ((_get_scale(ptr_low_scale) != _get_scale(ptr_high_scale)) &&
         !status_code) {
    if (_normalize_to_upper(ptr_low_scale)) {
      if (_get_scale(ptr_high_scale) == 0) {
        status_code = _get_sign(ptr_low_scale) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      // all things around that function should be plased inside of it
      _divide_and_round(ptr_high_scale, 0);
      _set_scale(ptr_high_scale, _get_scale(ptr_high_scale) - 1);
    }
  }

  // change the values to normolized saving order
  if (!status_code) {
    if (order) {
      *value_1 = *ptr_low_scale;
      *value_2 = *ptr_high_scale;
    } else {
      *value_2 = *ptr_low_scale;
      *value_1 = *ptr_high_scale;
    }
  }
  return status_code;
}

int _normalize_product(uint64_t product[6], int scale, int sign,
                       s21_decimal *result) {
  _init_decimal_zero(result);

  uint32_t last_digit = 0;
  int status_code = S21_SUCCESS;

  while ((product[3] || product[4] || product[5] || scale > 28) && scale > 0) {
    uint64_t remainder = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t dividend = (remainder << 32) | product[i];
      product[i] = dividend / 10;
      remainder = dividend % 10;
    }
    last_digit = remainder;
    scale--;
  }

  if (product[3] || product[4] || product[5]) {
    status_code = sign ? S21_TOO_SMALL : S21_TOO_LARGE;
  }

  if ((last_digit > 5 || (last_digit == 5 && (product[0] & 1))) &&
      !status_code) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal tmp = {{product[0], product[1], product[2], 0}};

    if (_add_with_carry(&tmp, &one, &tmp)) {
      status_code = sign ? S21_TOO_SMALL : S21_TOO_LARGE;
    }

    if (!status_code)
      for (int i = 0; i < 3; ++i) product[i] = tmp.bits[i];
  }

  if (!status_code) {
    for (int i = 0; i < 3; ++i) result->bits[i] = product[i];

    _set_sign(result, sign);
    _set_scale(result, scale);
  }

  return status_code;
}

int _divide_and_round(s21_decimal *dec, uint32_t carry) {
  const uint32_t dividend[4] = {dec->bits[0], dec->bits[1], dec->bits[2],
                                carry};
  uint32_t quotient[4] = {0, 0, 0, 0};
  uint64_t temp_carry = 0;

  for (int i = 3; i >= 0; i--) {
    temp_carry = (temp_carry << 32) | dividend[i];
    quotient[i] = (uint32_t)(temp_carry / 10);
    temp_carry %= 10;
  }

  if (temp_carry > 5 || (temp_carry == 5 && (quotient[0] & 1))) {
    s21_decimal temp_dec = {{quotient[0], quotient[1], quotient[2], 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    if (_add_with_carry(&temp_dec, &one, &temp_dec)) {
      quotient[3]++;
    }
    quotient[0] = temp_dec.bits[0];
    quotient[1] = temp_dec.bits[1];
    quotient[2] = temp_dec.bits[2];
  }

  dec->bits[0] = quotient[0];
  dec->bits[1] = quotient[1];
  dec->bits[2] = quotient[2];
  return quotient[3];
}

int _add_with_carry(const s21_decimal *x, const s21_decimal *y,
                    s21_decimal *result) {
  int carry = 0;
  for (int i = 0; i < 3; ++i) {
    uint64_t sum = (uint64_t)x->bits[i] + y->bits[i] + (carry ? 1 : 0);
    result->bits[i] = (uint32_t)sum;
    carry = (sum >> 32);
  }

  return carry;
}

int _normalize_to_upper(s21_decimal *to_normalize) {
  uint32_t carry = 0;
  s21_decimal tmp = *to_normalize;

  for (int i = 0; i < 3; ++i) {
    uint64_t product = (uint64_t)tmp.bits[i] * 10 + carry;
    tmp.bits[i] = (uint32_t)product;
    carry = product >> 32;
  }

  if (!carry) {
    *to_normalize = tmp;
    _set_scale(to_normalize, _get_scale(to_normalize) + 1);
  }

  return carry ? S21_ERROR : S21_SUCCESS;
}

int _compare_mantissas(const s21_decimal *value_1, const s21_decimal *value_2) {
  int resp = 0;
  for (int i = 2; i >= 0 && !resp; i--) {
    if ((uint32_t)value_1->bits[i] > (uint32_t)value_2->bits[i]) resp = 1;
    if ((uint32_t)value_1->bits[i] < (uint32_t)value_2->bits[i]) resp = -1;
  }
  return resp;
}

int _multiply_by_10(s21_decimal *value) {
  uint64_t temp;
  uint32_t carry = 0;

  temp = (uint64_t)value->bits[0] * 10 + carry;
  value->bits[0] = (uint32_t)temp;
  carry = (uint32_t)(temp >> 32);

  temp = (uint64_t)value->bits[1] * 10 + carry;
  value->bits[1] = (uint32_t)temp;
  carry = (uint32_t)(temp >> 32);

  temp = (uint64_t)value->bits[2] * 10 + carry;
  value->bits[2] = (uint32_t)temp;
  carry = (uint32_t)(temp >> 32);

  return carry != 0;
}

// Divide decimal by 10, return the remainder
uint32_t _divide_by_10(s21_decimal *value, uint32_t remainder) {
  uint64_t temp;

  temp = ((uint64_t)remainder << 32) | value->bits[2];
  value->bits[2] = (uint32_t)(temp / 10);
  remainder = (uint32_t)(temp % 10);

  temp = ((uint64_t)remainder << 32) | value->bits[1];
  value->bits[1] = (uint32_t)(temp / 10);
  remainder = (uint32_t)(temp % 10);

  temp = ((uint64_t)remainder << 32) | value->bits[0];
  value->bits[0] = (uint32_t)(temp / 10);
  remainder = (uint32_t)(temp % 10);

  return remainder;
}

meta_t _get_bit(s21_decimal *dec, unsigned order) {
  if (order < 32) {
    return (meta_t)((dec->bits[0] >> order) & 1);
  } else if (order < 64) {
    return (meta_t)((dec->bits[1] >> (order - 32)) & 1);
  } else {
    return (meta_t)((dec->bits[2] >> (order - 64)) & 1);
  }
}

void from_decimal_to_int192(s21_decimal value, s21_uint192_t *result) {
  for (int i = 0; i < 3; i++) result->bits[i] = value.bits[i];
  for (int i = 3; i < 6; i++) result->bits[i] = 0;
}

int uint192_compare(s21_uint192_t value1, s21_uint192_t value2) {
  for (int i = 5; i >= 0; --i) {
    if (value1.bits[i] > value2.bits[i]) return 1;
    if (value1.bits[i] < value2.bits[i]) return -1;
  }
  return 0;
}

/* void uint192_shift_left(s21_uint192_t *value, uint32_t shift) {
  uint32_t carry;
  for (int s = 0; s < shift; ++s) {
    uint32_t prev_carry = 0;
    for (int i = 0; i < 6; ++i) {
      carry = value->bits[i] >> 31;
      value->bits[i] = (value->bits[i] << 1) | prev_carry;
      prev_carry = carry;
    }
  }
}

void uint192_shift_right(s21_uint192_t *value, uint32_t shift) {
  uint32_t carry;
  for (int s = 0; s < shift; ++s) {
    uint32_t prev_carry = 0;
    for (int i = 5; i >= 0; --i) {
      carry = (value->bits[i] & 1) << 31;
      value->bits[i] = (value->bits[i] >> 1) | prev_carry << 31;
      prev_carry = carry;
    }
  }
}*/

uint32_t uint192_add(s21_uint192_t value1, s21_uint192_t value2,
                     s21_uint192_t *result) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; ++i) {
    uint64_t res = (uint64_t)value1.bits[i] + (uint64_t)value2.bits[i] + carry;
    result->bits[i] = (uint32_t)res;
    carry = res >> 32;
  }
  return carry;
}

void uint192_sub0(s21_uint192_t value1, s21_uint192_t value2,
                  s21_uint192_t *result) {
  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t diff = (uint64_t)value1.bits[i] - value2.bits[i] - borrow;
    result->bits[i] = (uint32_t)diff;

    borrow = (diff >> 63) & 1;
  }
}

int uint192_sub(s21_uint192_t value1, s21_uint192_t value2,
                s21_uint192_t *result) {
  int comp = uint192_compare(value1, value2);
  if (comp) {
    s21_uint192_t const *val1_ptr;
    s21_uint192_t const *val2_ptr;
    int sign_code = 1;

    if (comp > 0) {
      val1_ptr = &value1;
      val2_ptr = &value2;
    } else {
      val1_ptr = &value2;
      val2_ptr = &value1;
      sign_code = 0;
    }
    uint192_sub0(*val1_ptr, *val2_ptr, result);
    return sign_code;

  } else {
    for (int i = 0; i < 6; ++i) result->bits[i] = 0;
    return 1;
  }
}

int uint192_mult_by_10(s21_uint192_t *value) {
  uint32_t carry = 0;
  for (int i = 0; i < 6; ++i) {
    uint64_t temp = (uint64_t)value->bits[i] * 10 + carry;
    value->bits[i] = (uint32_t)temp;
    carry = (uint32_t)(temp >> 32);
  }
  return carry != 0;
}

uint32_t uint192_div_by_10(s21_uint192_t *value) {
  uint32_t reminder = 0;
  for (int i = 5; i >= 0; --i) {
    uint64_t temp = ((uint64_t)reminder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(temp / 10);
    reminder = (uint32_t)(temp % 10);
  }
  return reminder;
}

int leveling(s21_decimal value_1, s21_decimal value_2, s21_uint192_t *res1,
             s21_uint192_t *res2) {
  from_decimal_to_int192(value_1, res1);
  from_decimal_to_int192(value_2, res2);
  meta_t scale1 = _get_scale(&value_1);
  meta_t scale2 = _get_scale(&value_2);
  if (scale1 > scale2) {
    while (scale2 < scale1) {
      uint192_mult_by_10(res2);
      scale2++;
    }
  } else if (scale1 < scale2) {
    while (scale1 < scale2) {
      uint192_mult_by_10(res1);
      scale1++;
    }
  }
  return scale1;
}

int from_uint192_to_decimal(s21_uint192_t *src, meta_t scale,
                            s21_decimal *dst) {
  int code = S21_ERROR;
  if (src && dst) {
    while (src->bits[3] | src->bits[4] | src->bits[5]) {
      if (scale == 0) return S21_TOO_LARGE;
      uint32_t rem = uint192_div_by_10(src);
      if (rem > 5 || (rem == 5 && src->bits[0] << 31)) {
        s21_uint192_t one = {{1, 0, 0, 0, 0, 0}};
        uint192_add(*src, one, src);
      }
      scale--;
    }
    for (int i = 0; i < 3; ++i) {
      dst->bits[i] = src->bits[i];
    }
    _set_scale(dst, scale);
    code = S21_SUCCESS;
  }
  return code;
}