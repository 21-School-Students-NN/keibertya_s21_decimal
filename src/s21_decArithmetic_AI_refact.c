#include <stdio.h>

#include "../include/s21_helpers_func.h"
// =================================================================================
//                            ОСНОВНАЯ ФУНКЦИЯ СЛОЖЕНИЯ
// =================================================================================

Int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 1;  // Защита от NULL указателя
  *result = (s21_decimal){{0, 0, 0, 0}};

  // --- БЫСТРЫЙ ПУТЬ ---
  if (GET_SCALE(value_1) == GET_SCALE(value_2) &&
      GET_SIGN(value_1) == GET_SIGN(value_2)) {
    uInt carry = add_96_mantissas(value_1, value_2, result);
    if (carry == 0) {
      result->bits[3] = value_1.bits[3];
      return S21_SUCCESS;
    }
    // Если переполнение, проваливаемся в медленный путь
  }

  // --- МЕДЛЕННЫЙ ПУТЬ ---
  s21_decimal norm_val1 = value_1;
  s21_decimal norm_val2 = value_2;

  int norm_error = normalizing(&norm_val1, &norm_val2);
  if (norm_error) return norm_error;

  if (GET_SIGN(value_1) == GET_SIGN(value_2)) {  // СЛОЖЕНИЕ
    s21_decimal result_bits = {{0, 0, 0, 0}};
    uInt carry_out = add_96_mantissas(norm_val1, norm_val2, &result_bits);
    int final_scale = GET_SCALE(norm_val1);

    while (carry_out > 0) {
      if (final_scale <= 0) {
        return GET_SIGN(value_1) ? S21_NEGATIVE_INFINITY : S21_INFINITY;
      }
      final_scale--;
      carry_out = divide_and_round(&result_bits, carry_out);
    }

    result->bits[0] = result_bits.bits[0];
    result->bits[1] = result_bits.bits[1];
    result->bits[2] = result_bits.bits[2];
    SET_SIGN(result, GET_SIGN(value_1));
    SET_SCALE(result, final_scale);

  } else {  // ВЫЧИТАНИЕ (когда знаки разные)
    const s21_decimal *ptr_big, *ptr_small;

    // --- ИСПРАВЛЕНИЕ: Правильно определяем большее и меньшее число по модулю
    // ---
    if (compare_mantissas_96(norm_val1, norm_val2) >= 0) {
      ptr_big = &norm_val1;
      ptr_small = &norm_val2;
    } else {
      ptr_big = &norm_val2;
      ptr_small = &norm_val1;
    }

    // Вычитаем мантиссы "в столбик" с заемом
    uLong borrow = 0;
    for (int i = 0; i < 3; i++) {
      uLong diff = (uLong)ptr_big->bits[i] - ptr_small->bits[i] - borrow;
      result->bits[i] = (uInt)diff;
      // Если произошло заимствование, `diff` будет очень большим.
      // `diff >> 63` вернет 1, если был заем, и 0, если нет.
      borrow = (diff >> 63) & 1;
    }

    // Устанавливаем знак результата (он равен знаку большего по модулю числа)
    SET_SIGN(result, GET_SIGN(*ptr_big));
    // Устанавливаем масштаб (он уже нормализован)
    SET_SCALE(result, GET_SCALE(norm_val1));
  }

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0) {
    SET_SIGN(result, 0);
    SET_SCALE(result, 0);
  }

  return S21_SUCCESS;
}

// =================================================================================
//                            ОСНОВНАЯ ФУНКЦИЯ ВЫЧИТАНИЯ
// =================================================================================
Int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Чтобы вычесть value_2, мы инвертируем его знак и прибавляем.
  // A - B  <=>  A + (-B)

  // Инвертируем знаковый бит у value_2.
  // Для этого можно использовать операцию XOR с маской знакового бита,
  // или, что более наглядно с вашими макросами:
  SET_SIGN(&value_2, !GET_SIGN(value_2));

  // Вызываем вашу функцию сложения, которая сама определит,
  // что знаки разные, и выполнит вычитание.
  return s21_add(value_1, value_2, result);
}

/**
 * @brief Умножает два числа s21_decimal.
 *
 * @param value_1 Первый множитель.
 * @param value_2 Второй множитель.
 * @param result Указатель на s21_decimal, куда будет записан результат.
 * @return int Код ошибки (0 - OK, 1 - положительное переполнение, 2 -
 * отрицательное).
 */
uInt s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // ШАГ 0: Полностью обнуляем результат в самом начале.
  // Это ГАРАНТИРУЕТ, что мы никогда не будем работать с мусорными данными
  // в `result`, особенно в `result->bits[3]`.
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;

  // ШАГ 1: Быстрая проверка на ноль.
  // Если мантисса одного из чисел равна нулю, результат — уже готовый
  // канонический ноль, который мы установили на шаге 0.
  int is_value1_zero =
      (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0);
  int is_value2_zero =
      (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0);

  if (is_value1_zero || is_value2_zero) {
    return S21_SUCCESS;  // Результат уже {0,0,0,0}
  }

  // ШАГ 2: Вычисление метаданных.
  int result_sign = GET_SIGN(value_1) ^ GET_SIGN(value_2);
  int result_scale = GET_SCALE(value_1) + GET_SCALE(value_2);

  // ШАГ 3: Умножение мантисс.
  uint32_t temp_result[6] = {0};
  multiply_mantissas(value_1, value_2, temp_result);

  // ШАГ 4: Обработка результата.
  int is_overflowed = (temp_result[3] || temp_result[4] || temp_result[5]);

  if (!is_overflowed && result_scale <= 28) {
    // Сценарий 1: Идеальный случай.
    result->bits[0] = temp_result[0];
    result->bits[1] = temp_result[1];
    result->bits[2] = temp_result[2];

    // `result->bits[3]` уже 0, поэтому макросы отработают корректно.
    SET_SIGN(result, result_sign);
    SET_SCALE(result, result_scale);

    return S21_SUCCESS;
  } else {
    // Сценарий 2: Требуется нормализация.
    return normalize_and_fit(temp_result, result_scale, result_sign, result);
  }
}