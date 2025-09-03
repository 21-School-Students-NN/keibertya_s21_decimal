#include <stdio.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
// =================================================================================
//                            ОСНОВНАЯ ФУНКЦИЯ СЛОЖЕНИЯ
// =================================================================================

int32_t murk_add(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  if (!result) return 1;  // Защита от NULL указателя
  *result = (s21_decimal){{0, 0, 0, 0}};

  // --- БЫСТРЫЙ ПУТЬ ---
  if (_get_scale(&value_1) == _get_scale(&value_2) &&
      _get_sign(&value_1) == _get_sign(&value_2)) {
    uint32_t carry = add_96_mantissas(value_1, value_2, result);
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

  if (_get_sign(&value_1) == _get_sign(&value_2)) {  // СЛОЖЕНИЕ
    s21_decimal result_bits = {{0, 0, 0, 0}};
    uint32_t carry_out = add_96_mantissas(norm_val1, norm_val2, &result_bits);
    int final_scale = _get_scale(&norm_val1);

    while (carry_out > 0) {
      if (final_scale <= 0) {
        return _get_sign(&value_1) ? S21_TOO_SMALL : S21_TOO_LARGE;
      }
      final_scale--;
      carry_out = divide_and_round(&result_bits, carry_out);
    }

    result->bits[0] = result_bits.bits[0];
    result->bits[1] = result_bits.bits[1];
    result->bits[2] = result_bits.bits[2];
    _set_sign(result, _get_sign(&value_1));
    _set_scale(result, final_scale);

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
    uint64_t borrow = 0;
    for (int i = 0; i < 3; i++) {
      uint64_t diff = (uint64_t)ptr_big->bits[i] - ptr_small->bits[i] - borrow;
      result->bits[i] = (uint32_t)diff;
      // Если произошло заимствование, `diff` будет очень большим.
      // `diff >> 63` вернет 1, если был заем, и 0, если нет.
      borrow = (diff >> 63) & 1;
    }

    // Устанавливаем знак результата (он равен знаку большего по модулю числа)
    _set_sign(result, _get_sign(ptr_big));
    // Устанавливаем масштаб (он уже нормализован)
    _set_scale(result, _get_scale(&norm_val1));
  }

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0) {
    _set_sign(result, 0);
    _set_scale(result, 0);
  }

  return S21_SUCCESS;
}

// =================================================================================
//                            ОСНОВНАЯ ФУНКЦИЯ ВЫЧИТАНИЯ
// =================================================================================
int32_t murk_sub(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  // Чтобы вычесть value_2, мы инвертируем его знак и прибавляем.
  // A - B  <=>  A + (-B)

  // Инвертируем знаковый бит у value_2.
  // Для этого можно использовать операцию XOR с маской знакового бита,
  // или, что более наглядно с вашими макросами:
  _set_sign(&value_2, !_get_sign(&value_2));

  // Вызываем вашу функцию сложения, которая сама определит,
  // что знаки разные, и выполнит вычитание.
  return murk_add(value_1, value_2, result);
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
int murk_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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
  int result_sign = _get_sign(&value_1) ^ _get_sign(&value_2);
  int result_scale = _get_scale(&value_1) + _get_scale(&value_2);

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
    _set_sign(result, result_sign);
    _set_scale(result, result_scale);

    return S21_SUCCESS;
  } else {
    // Сценарий 2: Требуется нормализация.
    return normalize_and_fit(temp_result, result_scale, result_sign, result);
  }
}