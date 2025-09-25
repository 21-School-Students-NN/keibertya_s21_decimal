#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = S21_ERROR;
  if (result && _get_scale(&value) <= 28) {
    *result = value;
    result->bits[3] ^= (1u << 31);
    error = S21_SUCCESS;
  }
  return error;
}