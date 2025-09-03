#include <stdio.h>

#include "../include/s21_helpers.h"
// =================================================================================
//                    ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (НИЗКИЙ УРОВЕНЬ)
// =================================================================================

int32_t compare_mantissas_96(const s21_decimal value_1,
                             const s21_decimal value_2) {
  for (int i = 2; i >= 0; i--) {
    // --- ИСПРАВЛЕНИЕ: Сравниваем как беззнаковые числа ---
    if ((uint32_t)value_1.bits[i] > (uint32_t)value_2.bits[i]) return 1;
    if ((uint32_t)value_1.bits[i] < (uint32_t)value_2.bits[i]) return -1;
  }
  return 0;
}

uint32_t add_96_mantissas(const s21_decimal num1, const s21_decimal num2,
                          s21_decimal *result) {
  uint64_t sum = (uint64_t)num1.bits[0] + num2.bits[0];
  result->bits[0] = (uint32_t)sum;
  sum = (uint64_t)num1.bits[1] + num2.bits[1] + (sum >> 32);
  result->bits[1] = (uint32_t)sum;
  sum = (uint64_t)num1.bits[2] + num2.bits[2] + (sum >> 32);
  result->bits[2] = (uint32_t)sum;
  return (uint32_t)(sum >> 32);
}

uint8_t increment_96(s21_decimal *num) {
  uint32_t tmp_num[3];
  tmp_num[0] = num->bits[0];
  tmp_num[1] = num->bits[1];
  tmp_num[2] = num->bits[2];
  uint8_t error_fl = 0;
  uint64_t summ = (uint64_t)tmp_num[0] + 1;
  tmp_num[0] = (uint32_t)summ;
  if ((uint32_t)(summ >> 32)) {
    summ = (uint64_t)tmp_num[1] + 1;
    tmp_num[1] = (uint32_t)summ;
    if ((uint32_t)(summ >> 32)) {
      summ = (uint64_t)tmp_num[2] + 1;
      tmp_num[2] = (uint32_t)summ;
      if ((uint32_t)(summ >> 32)) error_fl = 1;
    }
  }
  // if (!error_fl) {
  num->bits[0] = tmp_num[0];
  num->bits[1] = tmp_num[1];
  num->bits[2] = tmp_num[2];
  //}
  return error_fl;
}

int multiply_96_mantissa(s21_decimal *num) {
  uint32_t x[3] = {num->bits[0], num->bits[1], num->bits[2]};
  // x*2
  uint32_t x2[3];
  uint64_t carry = (uint64_t)x[0] << 1;
  x2[0] = (uint32_t)carry;
  carry = ((uint64_t)x[1] << 1) | (carry >> 32);
  x2[1] = (uint32_t)carry;
  carry = ((uint64_t)x[2] << 1) | (carry >> 32);
  x2[2] = (uint32_t)carry;
  if (carry >> 32) return S21_TRUE;
  // x*8
  uint32_t x8[3];
  carry = (uint64_t)x[0] << 3;
  x8[0] = (uint32_t)carry;
  carry = ((uint64_t)x[1] << 3) | (carry >> 32);
  x8[1] = (uint32_t)carry;
  carry = ((uint64_t)x[2] << 3) | (carry >> 32);
  x8[2] = (uint32_t)carry;
  if (carry >> 32) return S21_TRUE;
  // x*10 = x*2 + x*8
  s21_decimal tmp_dec_x2 = {{x2[0], x2[1], x2[2], 0}};
  s21_decimal tmp_dec_x8 = {{x8[0], x8[1], x8[2], 0}};
  return add_96_mantissas(tmp_dec_x2, tmp_dec_x8, num) > 0;
}

uint32_t divide_and_round(s21_decimal *num, uint32_t carry_in) {
  const uint32_t dividend[4] = {num->bits[0], num->bits[1], num->bits[2],
                                carry_in};
  uint32_t quotient[4] = {0, 0, 0, 0};
  uint64_t temp_carry = 0;

  for (int i = 3; i >= 0; i--) {
    temp_carry = (temp_carry << 32) | dividend[i];
    quotient[i] = (uint32_t)(temp_carry / 10);
    temp_carry %= 10;
  }

  if (temp_carry > 5 || (temp_carry == 5 && (quotient[0] & 1))) {
    s21_decimal temp_dec = {{quotient[0], quotient[1], quotient[2], 0}};
    if (increment_96(&temp_dec)) {
      quotient[3]++;
    }
    quotient[0] = temp_dec.bits[0];
    quotient[1] = temp_dec.bits[1];
    quotient[2] = temp_dec.bits[2];
  }

  num->bits[0] = quotient[0];
  num->bits[1] = quotient[1];
  num->bits[2] = quotient[2];
  return quotient[3];
}

uint32_t normalizing(s21_decimal *num1, s21_decimal *num2) {
  s21_decimal *ptr_low_scale, *ptr_high_scale;
  if (_get_scale(num1) < _get_scale(num2)) {
    ptr_low_scale = num1;
    ptr_high_scale = num2;
  } else {
    ptr_low_scale = num2;
    ptr_high_scale = num1;
  }
  while (_get_scale(ptr_low_scale) != _get_scale(ptr_high_scale)) {
    if (!multiply_96_mantissa(ptr_low_scale)) {
      _set_scale(ptr_low_scale, _get_scale(ptr_low_scale) + 1);
    } else {
      if (_get_scale(ptr_high_scale) == 0) {
        return _get_sign(ptr_low_scale) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      if (ptr_high_scale->bits[0] == 0 && ptr_high_scale->bits[1] == 0 &&
          ptr_high_scale->bits[2] == 0) {
        return _get_sign(ptr_low_scale) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      uint32_t carry_zero = 0;
      divide_and_round(ptr_high_scale, carry_zero);
      _set_scale(ptr_high_scale, _get_scale(ptr_high_scale) - 1);
    }
  }
  return S21_SUCCESS;
}

void multiply_mantissas(s21_decimal value_1, s21_decimal value_2,
                        uint32_t temp_result[6]) {
  // Алгоритм имитирует умножение чисел в столбик, как в школе.
  // Мы умножаем каждую 32-битную "цифру" (bits[i]) первого числа
  // на каждую "цифру" второго (bits[j]).
  for (int i = 0; i < 3; i++) {
    uint64_t carry = 0;  // Перенос внутри одного цикла (при умножении на одну
                         // "цифру" value_2)
    for (int j = 0; j < 3; j++) {
      // Позиция, в которую мы будем записывать результат.
      int pos = i + j;

      // Умножаем две 32-битные "цифры". Результат будет 64-битным.
      // Это самый важный шаг: мы используем uint64_t, чтобы избежать потери
      // данных.
      uint64_t product = (uint64_t)value_1.bits[i] * value_2.bits[j];

      // Теперь добавляем это произведение к нашему промежуточному 192-битному
      // результату. Складываем с тем, что уже лежит в temp_result[pos], и с
      // переносом от предыдущей итерации.
      uint64_t sum = (uint64_t)temp_result[pos] + product + carry;

      // Записываем младшие 32 бита суммы в текущую позицию.
      temp_result[pos] = sum & 0xFFFFFFFF;  // Эквивалентно sum % (2^32)

      // Сохраняем старшие 32 бита как перенос для следующей итерации.
      carry = sum >> 32;  // Эквивалентно sum / (2^32)
    }

    // Если после умножения на все "цифры" value_2 остался перенос,
    // добавляем его в следующую позицию.
    if (carry > 0) {
      temp_result[i + 3] += carry;
    }
  }
}

uint32_t normalize_and_fit(uint32_t temp_result[6], int scale, int sign,
                           s21_decimal *result) {
  uint32_t last_digit = 0;

  while ((temp_result[3] || temp_result[4] || temp_result[5] || scale > 28) &&
         scale > 0) {
    uint64_t remainder = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t dividend = (remainder << 32) | temp_result[i];
      temp_result[i] = dividend / 10;
      remainder = dividend % 10;
    }
    last_digit = remainder;
    scale--;
  }

  if (temp_result[3] || temp_result[4] || temp_result[5]) {
    return sign ? S21_TOO_SMALL : S21_TOO_LARGE;
  }

  // --- ИСПРАВЛЕНИЕ: Округляем temp_result, а не result ---
  if (last_digit > 5) {
    // Прибавляем 1 к 96-битной мантиссе в temp_result
    for (int i = 0; i < 3; i++) {
      uint64_t sum = (uint64_t)temp_result[i] + 1;
      temp_result[i] = sum & 0xFFFFFFFF;
      if ((sum >> 32) == 0) break;
    }
  } else if (last_digit == 5) {
    if ((temp_result[0] & 1) != 0) {  // Проверяем последнюю цифру в temp_result
      for (int i = 0; i < 3; i++) {
        uint64_t sum = (uint64_t)temp_result[i] + 1;
        temp_result[i] = sum & 0xFFFFFFFF;
        if ((sum >> 32) == 0) break;
      }
    }
  }
  // --- КОНЕЦ ИСПРАВЛЕНИЯ ---

  // Записываем финальный результат.
  result->bits[0] = temp_result[0];
  result->bits[1] = temp_result[1];
  result->bits[2] = temp_result[2];
  result->bits[3] = 0;
  _set_sign(result, sign);
  _set_scale(result, scale);

  return S21_SUCCESS;
}