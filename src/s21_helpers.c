#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

unsigned char s21_get_scale(const s21_decimal *dec) {
    if (!dec) return S21_ERROR;
    return (unsigned char)((dec->bits[3] & SCALE_MASK) >> 16);
}

char s21_set_scale(s21_decimal *dec, const unsigned char scale) {
    if (scale > 28 || !dec) return S21_ERROR;
    dec->bits[3] = (dec->bits[3] & SIGN_MASK) | ((uint32_t)scale << 16);
    return S21_SUCCESS;
}

unsigned char s21_get_sign(const s21_decimal *dec) {
    if (!dec) return S21_ERROR;
    return (unsigned char)((dec->bits[3] & SIGN_MASK) >> 31);
}

char s21_set_sign(s21_decimal *dec, const unsigned char sign) {
    if (sign > 1 || !dec) return S21_ERROR;
    dec->bits[3] = (dec->bits[3] & SCALE_MASK) | ((uint32_t)sign << 31);
    return S21_SUCCESS;
}