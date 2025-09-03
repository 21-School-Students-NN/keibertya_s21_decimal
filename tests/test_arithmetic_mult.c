#include <stdio.h>
#include <string.h>

#include "../include/murk_helpers.h"
#include "../include/s21_suites.h"
// --- Вспомогательные функции для тестов ---

// Создает s21_decimal из int, знака и масштаба
void s21_decimal_from_int(int value, int sign, int scale, s21_decimal *result) {
  for (int i = 0; i < 4; i++) result->bits[i] = 0;
  if (value < 0) value = -value;
  result->bits[0] = value;
  if (sign) SET_SIGN(result, 1);
  if (scale) SET_SCALE(result, scale);
}

// Сравнивает два s21_decimal. Возвращает 1 если равны, 0 если нет.
// int s21_decimal_is_equal(s21_decimal d1, s21_decimal d2) {
//   return (d1.bits[0] == d2.bits[0] && d1.bits[1] == d2.bits[1] &&
//           d1.bits[2] == d2.bits[2] && d1.bits[3] == d2.bits[3]);
// }

int s21_decimal_is_equal(s21_decimal d1, s21_decimal d2) {
  int are_equal = (d1.bits[0] == d2.bits[0] && d1.bits[1] == d2.bits[1] &&
                   d1.bits[2] == d2.bits[2] && d1.bits[3] == d2.bits[3]);

  // Если они не равны, напечатаем их содержимое, чтобы увидеть разницу
  if (!are_equal) {
    printf("\n--- s21_decimal_is_equal FAILED ---\n");
    printf("RESULT:   bits[0]=%u, bits[1]=%u, bits[2]=%u, bits[3]=%u\n",
           (uint32_t)d1.bits[0], (uint32_t)d1.bits[1], (uint32_t)d1.bits[2],
           (uint32_t)d1.bits[3]);
    printf("EXPECTED: bits[0]=%u, bits[1]=%u, bits[2]=%u, bits[3]=%u\n",
           (uint32_t)d2.bits[0], (uint32_t)d2.bits[1], (uint32_t)d2.bits[2],
           (uint32_t)d2.bits[3]);
    printf("------------------------------------\n");
  }

  return are_equal;
}

// --- Тестовые случаи ---

// Тест #1: Умножение на ноль
START_TEST(test_mul_positive_by_zero) {
  s21_decimal val1, val2, result, expected;

  s21_decimal_from_int(123, 0, 0, &val1);
  s21_decimal_from_int(0, 0, 0, &val2);
  s21_decimal_from_int(0, 0, 0, &expected);

  int return_code = s21_mul(val1, val2, &result);
  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

// Тест #2: Ноль умножается на число
START_TEST(test_mul_zero_by_negative) {
  s21_decimal val1, val2, result, expected;

  s21_decimal_from_int(0, 0, 0, &val1);
  s21_decimal_from_int(-456, 1, 5, &val2);  // Можно любой масштаб
  s21_decimal_from_int(0, 0, 0, &expected);

  int return_code = s21_mul(val1, val2, &result);
  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_one) {
  s21_decimal val1, val2, result, expected;

  // 123.456 * 1 = 123.456
  s21_decimal_from_int(123456, 0, 3, &val1);
  s21_decimal_from_int(1, 0, 0, &val2);
  s21_decimal_from_int(123456, 0, 3, &expected);

  int return_code = s21_mul(val1, val2, &result);
  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);

  // 123.456 * (-1) = -123.456
  s21_decimal_from_int(1, 1, 0, &val2);
  s21_decimal_from_int(123456, 1, 3, &expected);

  return_code = s21_mul(val1, val2, &result);
  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_signs) {
  s21_decimal val1, val2, result, expected;

  // pos * pos = pos (5 * 10 = 50)
  s21_decimal_from_int(5, 0, 0, &val1);
  s21_decimal_from_int(10, 0, 0, &val2);
  s21_decimal_from_int(50, 0, 0, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);

  // pos * neg = neg (5 * -10 = -50)
  s21_decimal_from_int(10, 1, 0, &val2);
  s21_decimal_from_int(50, 1, 0, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);

  // neg * neg = pos (-5 * -10 = 50)
  s21_decimal_from_int(5, 1, 0, &val1);
  s21_decimal_from_int(50, 0, 0, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_with_scale) {
  s21_decimal val1, val2, result, expected;

  // 1.5 * 1.5 = 2.25
  s21_decimal_from_int(15, 0, 1, &val1);
  s21_decimal_from_int(15, 0, 1, &val2);
  s21_decimal_from_int(225, 0, 2, &expected);  // scale 1 + 1 = 2

  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

/**
 * Тестирует переполнение мантиссы и масштаба с последующей нормализацией.
 *
 * Сценарий:
 * 1. Берем максимальную мантиссу (2^96 - 1) с максимальным масштабом (28).
 * 2. Умножаем ее на 1.0 (мантисса 10, масштаб 1).
 * 3. Промежуточный результат:
 *    - Мантисса: (2^96 - 1) * 10, что очевидно больше 96 бит.
 *    - Масштаб: 28 + 1 = 29, что больше 28.
 * 4. Ожидаемое поведение функции normalize_and_fit:
 *    - Так как масштаб > 28, запускается деление на 10.
 *    - Новая мантисса = ((2^96 - 1) * 10) / 10 = 2^96 - 1.
 *    - Новый масштаб = 29 - 1 = 28.
 *    - Остаток от деления равен 0, округление не требуется.
 * 5. Итоговый результат должен быть равен исходному числу (максимальная
 * мантисса, масштаб 28).
 */
START_TEST(test_mul_overflow_and_clean_normalization) {
  s21_decimal val1, val2, result, expected;

  // --- ИСПРАВЛЕНИЕ: Полная инициализация переменных ---
  memset(&val1, 0, sizeof(s21_decimal));
  memset(&expected, 0, sizeof(s21_decimal));
  // ----------------------------------------------------

  // val1 = максимальное значение мантиссы
  val1.bits[0] = 0xFFFFFFFF;
  val1.bits[1] = 0xFFFFFFFF;
  val1.bits[2] = 0xFFFFFFFF;
  // Теперь val1.bits[3] равен 0, и макрос может его безопасно читать
  SET_SCALE(&val1, 28);

  // val2 = 1.0
  s21_decimal_from_int(10, 0, 1, &val2);

  // Ожидаемый результат должен быть идентичен val1
  expected = val1;

  int return_code = s21_mul(val1, val2, &result);

  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

/**
 * Тестирует переполнение с нормализацией и банковским округлением.
 *
 * Сценарий:
 * 1. Берем число 1.5 со scale=28 (мантисса 15, масштаб 29 не влезет).
 *    val1 = 0.0...015
 * 2. Умножаем на 10.0 (мантисса 10, масштаб 1).
 * 3. Промежуточный результат:
 *    - Мантисса: 15 * 10 = 150.
 *    - Масштаб: 28 + 1 = 29.
 * 4. Ожидаемое поведение функции normalize_and_fit:
 *    - Так как масштаб > 28, запускается деление на 10.
 *    - Новая мантисса = 150 / 10 = 15.
 *    - Остаток = 0. Округления нет.
 *    - Новый масштаб = 29 - 1 = 28.
 * 5. Итоговый результат: мантисса 15, масштаб 28.
 */
START_TEST(test_mul_overflow_and_normalization_simple) {
  s21_decimal val1, val2, result, expected;

  s21_decimal_from_int(15, 0, 28, &val1);
  s21_decimal_from_int(10, 0, 1, &val2);  // 10.0

  // Ожидаемый результат: 15 с масштабом 28
  s21_decimal_from_int(15, 0, 28, &expected);

  int return_code = s21_mul(val1, val2, &result);

  ck_assert_int_eq(return_code, S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_rounding_bankers) {
  s21_decimal val1, val2, result, expected;

  // Тест 1: отброшенная цифра 5, последняя оставшаяся - нечетная (1). Округляем
  // вверх. 1.5 * 0.1 с большим scale, чтобы вызвать нормализацию.
  s21_decimal_from_int(15, 0, 28, &val1);  // 0.0...15
  s21_decimal_from_int(1, 0, 1, &val2);    // 0.1
  // Промежуточный результат: мантисса 15, scale 29.
  // normalize: делим на 10 -> мантисса 1, остаток 5.
  // Последняя цифра (1) нечетная, округляем до 2.
  s21_decimal_from_int(2, 0, 28, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);

  // Тест 2: отброшенная цифра 5, последняя оставшаяся - четная (2). Округляем
  // вниз (отбрасываем). 2.5 * 0.1 с большим scale
  s21_decimal_from_int(25, 0, 28, &val1);
  // Промежуточный результат: мантисса 25, scale 29.
  // normalize: делим на 10 -> мантисса 2, остаток 5.
  // Последняя цифра (2) четная, отбрасываем остаток.
  s21_decimal_from_int(2, 0, 28, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_rounding_simple) {
  s21_decimal val1, val2, result, expected;

  // Тест 1: отброшенная цифра > 5 (7). Округляем вверх.
  // 1.7 * 0.1 с большим scale
  s21_decimal_from_int(17, 0, 28, &val1);
  s21_decimal_from_int(1, 0, 1, &val2);
  // Промежуточный результат: мантисса 17, scale 29.
  // normalize: делим на 10 -> мантисса 1, остаток 7. Округляем до 2.
  s21_decimal_from_int(2, 0, 28, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);

  // Тест 2: отброшенная цифра < 5 (2). Округляем вниз.
  // 1.2 * 0.1 с большим scale
  s21_decimal_from_int(12, 0, 28, &val1);
  // Промежуточный результат: мантисса 12, scale 29.
  // normalize: делим на 10 -> мантисса 1, остаток 2. Отбрасываем.
  s21_decimal_from_int(1, 0, 28, &expected);
  ck_assert_int_eq(s21_mul(val1, val2, &result), S21_SUCCESS);
  ck_assert_int_eq(s21_decimal_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_infinity) {
  s21_decimal max_dec, val, result;

  // --- ИСПРАВЛЕНИЕ: Полная инициализация ---
  memset(&max_dec, 0, sizeof(s21_decimal));
  // ------------------------------------------

  // MAX_DECIMAL = 2^96 - 1, scale 0
  max_dec.bits[0] = 0xFFFFFFFF;
  max_dec.bits[1] = 0xFFFFFFFF;
  max_dec.bits[2] = 0xFFFFFFFF;
  // max_dec.bits[3] уже 0 после memset, SET_SCALE не нужен.

  // MAX_DECIMAL * 2 -> S21_INFINITY
  s21_decimal_from_int(2, 0, 0, &val);
  ck_assert_int_eq(s21_mul(max_dec, val, &result), S21_INFINITY);

  // MAX_DECIMAL * (-2) -> S21_NEGATIVE_INFINITY
  s21_decimal_from_int(2, 1, 0, &val);
  ck_assert_int_eq(s21_mul(max_dec, val, &result), S21_NEGATIVE_INFINITY);
}
END_TEST

// --- Функция для создания набора тестов ---

Suite *mul_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mul");
  tc_core = tcase_create("Core");

  // Добавляем тесты в набор
  tcase_add_test(tc_core, test_mul_positive_by_zero);
  tcase_add_test(tc_core, test_mul_zero_by_negative);
  tcase_add_test(tc_core, test_mul_one);
  tcase_add_test(tc_core, test_mul_signs);
  tcase_add_test(tc_core, test_mul_with_scale);
  tcase_add_test(tc_core, test_mul_overflow_and_normalization_simple);
  tcase_add_test(tc_core, test_mul_overflow_and_clean_normalization);
  tcase_add_test(tc_core, test_mul_rounding_bankers);
  tcase_add_test(tc_core, test_mul_rounding_simple);
  tcase_add_test(tc_core, test_mul_infinity);

  suite_add_tcase(s, tc_core);
  return s;
}
