#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = S21_ERROR;
  int scale = _get_scale(&value);
  if (result && scale <= 28) {
    if (!s21_truncate(value, result)) {
      error = S21_SUCCESS;
      if (s21_is_less(value, *result)) {
        s21_decimal one = {{1u, 0u, 0u, 0u}};
        error = s21_sub(*result, one, result) ? S21_ERROR : S21_SUCCESS;
      }
    }
  }
  return error;
}