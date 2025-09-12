#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int shift_left(s21_decimal *value) {
  uint32_t carry = 0;
  uint32_t new_carry;

  new_carry = (value->bits[0] >> 31) & 1;
  value->bits[0] = (value->bits[0] << 1);

  carry = new_carry;

  new_carry = (value->bits[1] >> 31) & 1;
  value->bits[1] = (value->bits[1] << 1) | carry;
  carry = new_carry;

  new_carry = (value->bits[2] >> 31) & 1;
  value->bits[2] = (value->bits[2] << 1) | carry;

  return new_carry != 0;  // overflow
}

void shift_right(s21_decimal *value) {
  uint32_t carry = 0;
  uint32_t new_carry;

  new_carry = value->bits[2] & 1;
  value->bits[2] = (value->bits[2] >> 1);  // | (carry << 31);
  carry = new_carry;

  new_carry = value->bits[1] & 1;
  value->bits[1] = (value->bits[1] >> 1) | (carry << 31);
  carry = new_carry;

  value->bits[0] = (value->bits[0] >> 1) | (carry << 31);
}

// Dividing mantissas to obtain the quotient and remainder
int divide_mantissas(s21_decimal dividend, s21_decimal divisor,
                     s21_decimal *quotient, s21_decimal *remainder) {
  if (_is_decimal_zero(&divisor)) {
    return S21_DIV_BY_ZERO;  // Деление на ноль
  }

  _init_decimal_zero(quotient);
  *remainder = dividend;

  s21_decimal temp;
  int bits_to_shift = 0;

  // Нормализуем делитель (сдвигаем влево пока старший бит не станет 1 или не
  // дойдем до делителя по ширине)
  while (!(divisor.bits[2] & 0x80000000) &&
         _compare_mantissas(&divisor, remainder) <= 0) {
    if (shift_left(&divisor)) {
      break;  // Переполнение при сдвиге
    }
    bits_to_shift++;
  }

  // Деление в столбик
  for (int i = 0; i <= bits_to_shift; i++) {
    shift_left(quotient);

    if (_compare_mantissas(remainder, &divisor) >= 0) {
      // s21_sub(*remainder, divisor, &temp);
      _subtract_mantissas(*remainder, divisor, &temp);
      *remainder = temp;
      quotient->bits[0] |= 1;  // Устанавливаем младший бит
    }

    if (i < bits_to_shift) {
      shift_right(&divisor);
    }
  }

  return 0;
}

// Нормализация результата (уменьшение масштаба с округлением)
int normalize_result(s21_decimal *result, int scale_reduction) {
  uint32_t remainder = 0;
  int current_scale = _get_scale(result);

  for (int i = 0; i < scale_reduction; i++) {
    if (current_scale == 0) {
      return 1;  // Невозможно уменьшить масштаб дальше
    }

    remainder = _divide_by_10(result, remainder);
    current_scale--;
  }

  // Применяем банковское округление
  _bank_round(result, remainder, current_scale);
  _set_scale(result, current_scale);

  return 0;
}

// Основная функция деления
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) {
    return 1;
  }

  if (_is_decimal_zero(&value_2)) {
    return S21_DIV_BY_ZERO;
  }

  _init_decimal_zero(result);

  if (_is_decimal_zero(&value_1)) {
    // 0 / anything = 0
    return 0;
  }

  // Определяем знак результата
  int sign1 = _get_sign(&value_1);
  int sign2 = _get_sign(&value_2);
  int result_sign = sign1 ^ sign2;

  // Убираем знаки для работы с мантиссами
  _set_sign(&value_1, 0);
  _set_sign(&value_2, 0);

  int scale1 = _get_scale(&value_1);
  int scale2 = _get_scale(&value_2);
  int result_scale = 0;

  s21_decimal quotient = {0};
  s21_decimal remainder = {0};
  // Делим мантиссы
  if (divide_mantissas(value_1, value_2, &quotient, &remainder) != 0) {
    return 1;
  }
  // Вычисляем итоговый масштаб
  result_scale = scale1 - scale2;
  if (result_scale < 0) {
    result_scale = 0;
  }

  // Если есть остаток, добавляем десятичные разряды
  if (!_is_decimal_zero(&remainder) && result_scale < MAX_SCALE) {
    s21_decimal temp_quotient = quotient;
    s21_decimal temp_remainder = remainder;
    int precision = 0;

    // Добавляем десятичные разряды пока есть остаток и не достигнут максимум
    while (!_is_decimal_zero(&temp_remainder) &&
           (precision + result_scale) < MAX_SCALE &&
           precision < MAX_PRECISION) {
      // Умножаем остаток на 10
      _multiply_by_10(&temp_remainder);
      // Делим снова
      s21_decimal new_quotient = {0};
      s21_decimal new_remainder = {0};

      if (divide_mantissas(temp_remainder, value_2, &new_quotient,
                           &new_remainder) != 0) {
        break;
      }

      _multiply_by_10(&temp_quotient);

      s21_add(temp_quotient, new_quotient, &temp_quotient);
      temp_remainder = new_remainder;
      precision++;
    }

    quotient = temp_quotient;
    remainder = temp_remainder;
    result_scale += precision;
  }

  // Применяем банковское округление если нужно
  if (!_is_decimal_zero(&remainder) && result_scale >= MAX_SCALE) {
    // Конвертируем остаток в цифру для округления
    s21_decimal temp = remainder;
    uint32_t round_digit = 0;

    for (int i = 0; i < 10 && !_is_decimal_zero(&temp); i++) {
      round_digit = _divide_by_10(&temp, 0);
    }

    _bank_round(&quotient, round_digit, result_scale);
  }

  // Устанавливаем результат
  *result = quotient;
  _set_sign(result, result_sign);
  _set_scale(result, result_scale);

  // Проверяем переполнение масштаба
  if (result_scale > MAX_SCALE) {
    return normalize_result(result, result_scale - MAX_SCALE);
  }

  return 0;
}
