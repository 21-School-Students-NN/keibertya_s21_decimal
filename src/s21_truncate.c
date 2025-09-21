#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

/**
 * @brief calculate result for devision by ten of
 * mantissa as 96-bit int
 * @param value pointer to devided decimal
 * @return error code S21_ERROR (1) or S21_SUCCESS (0)
 */
int true_mantissa_devision_by_ten(s21_decimal *value);

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_ERROR;
  }

  _init_decimal_zero(result);

  int error = S21_SUCCESS;

  meta_t scale = _get_scale(&value);
  if (scale > 28) {
    error = S21_ERROR;
  }
  if (!error) {
    *result = value;
    if (scale > 0) {
      _set_scale(result, 0);
      for (int i = 0; i < scale && !error; i++)
        error = true_mantissa_devision_by_ten(result);
    }
  }
  return error;
}

int true_mantissa_devision_by_ten(s21_decimal *value) {
  int code = S21_ERROR;
  if (value) {
    code = S21_SUCCESS;
    unsigned long long remainder = 0;
    // deviding every part of 3-int mantissa
    for (int j = 2; j >= 0; j--) {
      unsigned long long tmp =
          ((unsigned long long)remainder << 32) | value->bits[j];
      value->bits[j] = tmp / 10;
      remainder = tmp % 10;
    }
  }
  return code;
}
