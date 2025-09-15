#include "../include/s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_ERROR;
  }

  *result = value;
  result->bits[3] ^= (1u << 31);

  return S21_SUCCESS;
}