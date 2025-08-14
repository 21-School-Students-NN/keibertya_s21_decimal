#include "../include/s21_helpers.h"

#include "../include/s21_decimal.h"

//======================================================================
//  Sign & Scale
//======================================================================

meta_t _get_scale(const s21_decimal *dec) {
  return (meta_t)((dec->bits[3] & SCALE_MASK) >> 16);
}

int _set_scale(s21_decimal *dec, const meta_t scale) {
  if (scale > 28) return S21_ERROR;
  dec->bits[3] = (dec->bits[3] & SIGN_MASK) | ((uint32_t)scale << 16);
  return S21_SUCCESS;
}

meta_t _get_sign(const s21_decimal *dec) {
  return (meta_t)((dec->bits[3] & SIGN_MASK) >> 31);
}

int _set_sign(s21_decimal *dec, const meta_t sign) {
  if (sign > 1) return S21_ERROR;
  dec->bits[3] = (dec->bits[3] & SCALE_MASK) | ((uint32_t)sign << 31);
  return S21_SUCCESS;
}

//======================================================================
//  Other
//======================================================================

void _init_decimal_zero(s21_decimal *dec) {
  memset(dec, 0, sizeof(s21_decimal));
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