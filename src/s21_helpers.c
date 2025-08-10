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

#if 0
int _normalize_to_upper(s21_decimal *dec, int diff) {

  uint96_t mantisa = 0;

// way to learn, will the pow 10 overflow or not
bool is_mul_pow10_overflow(uint96_t num, uint8_t power) {
  uint64_t max_val = ((uint64_t)1 << 96) - 1;  // 2^96 - 1
  uint64_t pow10 = 1;

  for (uint8_t i = 0; i < power; ++i) {
    if (pow10 > max_val / 10) return true;  // Умножение на 10 вызовет переполнение
    pow10 *= 10;
  }

  uint96_t tmp = num;
  for (uint8_t i = 0; i < 3; ++i) {
    if (tmp.bits[i] > max_val / pow10) return true;
  }

  return false;
}


  static const 
}
#endif