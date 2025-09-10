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

int _is_zero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int _is_equal(s21_decimal value1, s21_decimal value2) {
  return value1.bits[0] == value2.bits[0] && value1.bits[1] == value2.bits[1] &&
         value1.bits[2] == value2.bits[2] && value1.bits[3] == value2.bits[3];
}

// void _print_decimal_debug(s21_decimal dec) {
//   printf("Sign: %d, Scale: %d, ", _get_sign(&dec), _get_scale(&dec));
//   printf("Mantissa: 0x%08X%08X%08X", dec.bits[2], dec.bits[1], dec.bits[0]);
//   printf(" Full: [0x%08X %08X %08X %08X]\n", dec.bits[3], dec.bits[2],
//          dec.bits[1], dec.bits[0]);
// }
//======================================================================
//  Mantissa helpers (96-bit) and normalization
//======================================================================

// int _compare_mantissas_96(const s21_decimal *a, const s21_decimal *b) {
//   for (int i = 2; i >= 0; --i) {
//     if ((uint32_t)a->bits[i] > (uint32_t)b->bits[i]) return 1;
//     if ((uint32_t)a->bits[i] < (uint32_t)b->bits[i]) return -1;
//   }
//   return 0;
// }

// static inline uint32_t _add_with_carry_u32(uint32_t carry_in, uint32_t x,
//                                            uint32_t y, uint32_t *out) {
//   uint64_t sum = (uint64_t)x + y + (carry_in ? 1u : 0u);
//   *out = (uint32_t)sum;
//   return (uint32_t)(sum >> 32);
// }

// uint32_t _add_mantissas_96(const s21_decimal *a, const s21_decimal *b,
//                            s21_decimal *sum) {
//   uint32_t carry = 0;
//   for (int i = 0; i < 3; ++i)
//     carry =
//         _add_with_carry_u32(carry, (uint32_t)a->bits[i],
//         (uint32_t)b->bits[i],
//                             (uint32_t *)&sum->bits[i]);
//   return carry;
// }

// int _multiply_mantissas_96(const s21_decimal *a, const s21_decimal *b,
//                            uint32_t out192[6]) {
//   for (int i = 0; i < 6; ++i) out192[i] = 0;
//   for (int i = 0; i < 3; ++i) {
//     uint64_t carry = 0;
//     for (int j = 0; j < 3; ++j) {
//       uint64_t prod =
//           (uint64_t)(uint32_t)a->bits[i] * (uint64_t)(uint32_t)b->bits[j];
//       uint64_t sum = (uint64_t)out192[i + j] + prod + carry;
//       out192[i + j] = (uint32_t)(sum & 0xFFFFFFFFu);
//       carry = sum >> 32;
//     }
//     out192[i + 3] += (uint32_t)carry;
//   }
//   return S21_SUCCESS;
// }

// uint32_t _divide_by_10_with_bank_round(s21_decimal *num, uint32_t carry_in) {
//   const uint32_t dividend[4] = {(uint32_t)num->bits[0],
//   (uint32_t)num->bits[1],
//                                 (uint32_t)num->bits[2], carry_in};
//   uint32_t quotient[4] = {0, 0, 0, 0};
//   uint64_t rem = 0;
//   for (int i = 3; i >= 0; --i) {
//     uint64_t cur = (rem << 32) | dividend[i];
//     quotient[i] = (uint32_t)(cur / 10);
//     rem = cur % 10;
//   }
//   if (rem > 5 || (rem == 5 && (quotient[0] & 1u))) {
//     uint64_t inc = (uint64_t)quotient[0] + 1u;
//     quotient[0] = (uint32_t)inc;
//     if (inc >> 32) {
//       inc = (uint64_t)quotient[1] + 1u;
//       quotient[1] = (uint32_t)inc;
//       if (inc >> 32) {
//         inc = (uint64_t)quotient[2] + 1u;
//         quotient[2] = (uint32_t)inc;
//         if (inc >> 32) quotient[3] += 1u;
//       }
//     }
//   }
//   num->bits[0] = (int)quotient[0];
//   num->bits[1] = (int)quotient[1];
//   num->bits[2] = (int)quotient[2];
//   return quotient[3];
// }

// int _normalize_scales_meet(s21_decimal *lower_scale,
//                            s21_decimal *higher_scale) {
//   s21_decimal *low = lower_scale;
//   s21_decimal *high = higher_scale;
//   if (_get_scale(low) > _get_scale(high)) {
//     s21_decimal *tmp = low;
//     low = high;
//     high = tmp;
//   }
//   while (_get_scale(low) != _get_scale(high)) {
//     // try multiply low by 10 (x2 + x8)
//     s21_decimal x2 = *low, x8 = *low, tmp = {{0}};
//     // x2
//     uint32_t carry = 0;
//     for (int i = 0; i < 3; ++i) {
//       uint64_t s = ((uint64_t)(uint32_t)x2.bits[i] << 1) + carry;
//       x2.bits[i] = (int)(s & 0xFFFFFFFFu);
//       carry = (uint32_t)(s >> 32);
//     }
//     if (carry) goto shrink_high;
//     // x8
//     carry = 0;
//     for (int i = 0; i < 3; ++i) {
//       uint64_t s = ((uint64_t)(uint32_t)x8.bits[i] << 3) + carry;
//       x8.bits[i] = (int)(s & 0xFFFFFFFFu);
//       carry = (uint32_t)(s >> 32);
//     }
//     if (carry) goto shrink_high;
//     // sum x2+x8 -> tmp
//     _init_decimal_zero(&tmp);
//     _add_mantissas_96(&x2, &x8, &tmp);
//     *low = tmp;
//     _set_scale(low, _get_scale(low) + 1);
//     continue;
//   shrink_high:
//     if (_get_scale(high) == 0) return S21_ERROR;
//     uint32_t new_carry = _divide_by_10_with_bank_round(high, 0);
//     (void)new_carry;
//     _set_scale(high, _get_scale(high) - 1);
//   }
//   return S21_SUCCESS;
// }

// int _normalize_and_fit_192_to_96(uint32_t temp192[6], meta_t *scale,
//                                  meta_t sign, s21_decimal *result) {
//   uint32_t last_digit = 0;
//   while ((temp192[3] || temp192[4] || temp192[5] || *scale > MAX_SCALE) &&
//          *scale > 0) {
//     uint64_t rem = 0;
//     for (int i = 5; i >= 0; --i) {
//       uint64_t cur = (rem << 32) | temp192[i];
//       temp192[i] = (uint32_t)(cur / 10);
//       rem = cur % 10;
//     }
//     last_digit = (uint32_t)rem;
//     *scale = (meta_t)(*scale - 1);
//   }
//   if (temp192[3] || temp192[4] || temp192[5])
//     return sign ? S21_TOO_SMALL : S21_TOO_LARGE;
//
//   if (last_digit > 5 || (last_digit == 5 && (temp192[0] & 1u))) {
//     for (int i = 0; i < 3; ++i) {
//       uint64_t s = (uint64_t)temp192[i] + 1u;
//       temp192[i] = (uint32_t)(s & 0xFFFFFFFFu);
//       if ((s >> 32) == 0) break;
//     }
//   }
//   result->bits[0] = (int)temp192[0];
//   result->bits[1] = (int)temp192[1];
//   result->bits[2] = (int)temp192[2];
//   result->bits[3] = 0;
//   _set_sign(result, sign);
//   _set_scale(result, *scale);
//   return S21_SUCCESS;
// }

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
