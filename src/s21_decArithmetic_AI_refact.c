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

  } else {  // ВЫЧИТАНИЕ
    const s21_decimal *ptr_big;
    s21_decimal *ptr_small;
    if (compare_mantissas_96(norm_val1, norm_val2) >= 0) {
      ptr_big = &norm_val1;
      ptr_small = &norm_val2;
    } else {
      ptr_big = &norm_val2;
      ptr_small = &norm_val1;
    }
    s21_decimal small_copy = *ptr_small;
    small_copy.bits[0] = ~small_copy.bits[0];
    small_copy.bits[1] = ~small_copy.bits[1];
    small_copy.bits[2] = ~small_copy.bits[2];

    increment_96(&small_copy);  // Используем вашу функцию

    add_96_mantissas(*ptr_big, small_copy, result);

    SET_SIGN(result, GET_SIGN(*ptr_big));
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