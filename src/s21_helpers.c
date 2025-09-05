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

int _is_decimal_zero(const s21_decimal *dec) {
  return !(dec->bits[0] | dec->bits[1] | dec->bits[2]);
}

//======================================================================
//  Arithmetic
//======================================================================

int _normalizing(s21_decimal *value_1, s21_decimal *value_2) {
  if (_get_scale(value_1) == _get_scale(value_2)) return S21_SUCCESS;
  if (value_1 == value_2) return S21_ERROR;

  // pointers for find lowest and highest scales
  s21_decimal *ptr_low_scale, *ptr_high_scale;

  // temporary value for changing
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

  int status_code = S21_SUCCESS;

  while ((_get_scale(ptr_low_scale) != _get_scale(ptr_high_scale)) &&
         !status_code) {
    if (_normalize_to_upper(ptr_low_scale)) {
      if (_get_scale(ptr_high_scale) == 0) {
        status_code = _get_sign(ptr_low_scale) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      // all things around that fnction should be plased inside of it
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