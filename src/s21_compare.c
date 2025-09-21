#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

/**
 * @brief Compares two s21_decimal numbers for equality.
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 == value_2), otherwise 0 (FALSE).
 */
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  // 1. Check for zero. +0 and -0 are considered equal.
  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return S21_TRUE;  // TRUE
  }

  // 2. If the signs are different and the numbers are not zeros, they cannot be
  // equal.
  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    return S21_FALSE;  // FALSE
  }

  // 3. If the scale is different, the numbers need to be reduced to the same
  // scale. For example, 1.2 (scale=1) and 1.20 (scale=2) should be equal.
  _normalize(&value_1, &value_2);

  // 4. After normalization, if the mantissas are equal, then the numbers are
  // equal. _compare_mantissas will return 0 if equal.
  return _compare_mantissas(&value_1, &value_2) == 0;
}

/**
 * @brief Compares two s21_decimal numbers (less than).
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 < value_2), otherwise 0 (FALSE).
 */
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;
  int sign1 = _get_sign(&value_1);
  int sign2 = _get_sign(&value_2);

  // If both numbers are zeros, they are equal, not less.
  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return S21_FALSE;  // FALSE
  }

  // If the signs are different: the negative is always less than the positive.
  if (sign1 != sign2) {
    is_less = (sign1 > sign2);  // sign1=1 (отриц), sign2=0 (полож) -> TRUE
  } else {
    // The signs are the same, normalize and compare the mantissas.
    _normalize(&value_1, &value_2);
    int mantissa_cmp = _compare_mantissas(&value_1, &value_2);

    if (sign1 == 0) {  // Both numbers are positive
      // For positive: 5 < 10 -> mantissa 5 < mantissa 10.
      if (mantissa_cmp == -1) {
        is_less = 1;
      }
    } else {  // Both numbers are negative
      // For negative ones: -10 < -5 -> mantissa 10 > mantissa 5.
      if (mantissa_cmp == 1) {
        is_less = 1;
      }
    }
  }

  return is_less;
}
/**
 * @brief Compares two s21_decimal numbers (greater than).
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 > value_2), otherwise 0 (FALSE).
 */
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  // a > b is the same as b < a
  return s21_is_less(value_2, value_1);
}

/**
 * @brief Compares two s21_decimal numbers (less than or equal to).
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 <= value_2), otherwise 0 (FALSE).
 */
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // a <= b is the same as NOT (a > b)
  return !s21_is_greater(value_1, value_2);
}

/**
 * @brief Compares two s21_decimal numbers (greater than or equal to).
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 >= value_2), otherwise 0 (FALSE).
 */
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // a >= b is the same as NOT (a < b)
  return !s21_is_less(value_1, value_2);
}

/**
 * @brief Compares two numbers s21_decimal (not equal).
 * @param value_1 is the first value to compare.
 * @param value_2 is the second value to compare.
 * @return int 1 if TRUE (value_1 != value_2), otherwise 0 (FALSE).
 */
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}