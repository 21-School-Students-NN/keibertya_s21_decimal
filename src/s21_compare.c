#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

/**
 * @brief Сравнивает два числа s21_decimal на равенство.
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 == value_2), иначе 0 (FALSE).
 */
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  // 1. Проверка на ноль. +0 и -0 считаются равными.
  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return 1;  // TRUE
  }

  // 2. Если знаки разные, а числа не нули, они не могут быть равны.
  if (_get_sign(&value_1) != _get_sign(&value_2)) {
    return 0;  // FALSE
  }

  // 3. Если scale разный, числа нужно привести к одному scale.
  // Например, 1.2 (scale=1) и 1.20 (scale=2) должны быть равны.
  _normalize(&value_1, &value_2);

  // 4. После нормализации, если мантиссы равны, то и числа равны.
  // _compare_mantissas вернет 0 в случае равенства.
  return _compare_mantissas(&value_1, &value_2) == 0;
}

/**
 * @brief Сравнивает два числа s21_decimal (меньше).
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 < value_2), иначе 0 (FALSE).
 */
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;
  int sign1 = _get_sign(&value_1);
  int sign2 = _get_sign(&value_2);

  // Если оба числа - нули, они равны, а не меньше.
  if (_is_decimal_zero(&value_1) && _is_decimal_zero(&value_2)) {
    return 0;  // FALSE
  }

  // Если знаки разные: отрицательное всегда меньше положительного.
  if (sign1 != sign2) {
    is_less = (sign1 > sign2);  // sign1=1 (отриц), sign2=0 (полож) -> TRUE
  } else {
    // Знаки одинаковы, нормализуем и сравниваем мантиссы.
    _normalize(&value_1, &value_2);
    int mantissa_cmp = _compare_mantissas(&value_1, &value_2);

    if (sign1 == 0) {  // Оба числа положительные
      // Для положительных: 5 < 10 -> мантисса 5 < мантиссы 10.
      if (mantissa_cmp == -1) {
        is_less = 1;
      }
    } else {  // Оба числа отрицательные
      // Для отрицательных: -10 < -5 -> мантисса 10 > мантиссы 5.
      if (mantissa_cmp == 1) {
        is_less = 1;
      }
    }
  }

  return is_less;
}
/**
 * @brief Сравнивает два числа s21_decimal (больше).
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 > value_2), иначе 0 (FALSE).
 */
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  // a > b это то же самое, что и b < a
  return s21_is_less(value_2, value_1);
}

/**
 * @brief Сравнивает два числа s21_decimal (меньше или равно).
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 <= value_2), иначе 0 (FALSE).
 */
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // a <= b это то же самое, что НЕ (a > b)
  return !s21_is_greater(value_1, value_2);
}

/**
 * @brief Сравнивает два числа s21_decimal (больше или равно).
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 >= value_2), иначе 0 (FALSE).
 */
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // a >= b это то же самое, что НЕ (a < b)
  return !s21_is_less(value_1, value_2);
}

/**
 * @brief Сравнивает два числа s21_decimal (не равно).
 * @param value_1 Первое значение для сравнения.
 * @param value_2 Второе значение для сравнения.
 * @return int 1 если TRUE (value_1 != value_2), иначе 0 (FALSE).
 */
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}