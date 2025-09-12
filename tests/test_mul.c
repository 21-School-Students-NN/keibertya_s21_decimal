#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

// Базовое умножение положительных чисел без переполнения
START_TEST(test_mul_basic_positive) {
  s21_decimal a = {{3, 0, 0, 0}};  // 3
  s21_decimal b = {{7, 0, 0, 0}};  // 7
  s21_decimal r;
  ck_assert_msg(s21_mul(a, b, &r) == S21_SUCCESS,
                "mul returned error on 3 * 7");
  ck_assert_uint_eq(r.bits[0], 21);
  ck_assert_uint_eq(_get_sign(&r), 0);
  ck_assert_uint_eq(_get_scale(&r), 0);
}
END_TEST

// Умножение на ноль
START_TEST(test_mul_by_zero) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal r;
  ck_assert_int_eq(s21_mul(a, b, &r), S21_SUCCESS);
  ck_assert_uint_eq(r.bits[0], 0);
  ck_assert_uint_eq(r.bits[1], 0);
  ck_assert_uint_eq(r.bits[2], 0);
}
END_TEST

// Проверка знаков
START_TEST(test_mul_signs) {
  s21_decimal pos = {{5, 0, 0, 0}};
  s21_decimal neg = {{5, 0, 0, 0x80000000}};
  s21_decimal r;

  // + * + = +
  ck_assert_int_eq(s21_mul(pos, pos, &r), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&r), 0);

  // + * - = -
  ck_assert_int_eq(s21_mul(pos, neg, &r), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&r), 1);

  // - * - = +
  ck_assert_int_eq(s21_mul(neg, neg, &r), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&r), 0);
}
END_TEST

// Перенос между словами при умножении (мантисса)
START_TEST(test_mul_carry_between_words) {
  s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal r;
  ck_assert_int_eq(s21_mul(a, b, &r), S21_SUCCESS);
  ck_assert_uint_eq(r.bits[0], 0xFFFFFFFE);
  ck_assert_uint_eq(r.bits[1], 1);  // Перенос в старшее слово
}
END_TEST

// Умножение с масштабами (scale)
START_TEST(test_mul_scale) {
  // 1.23 * 4.0 = 4.92
  s21_decimal a = {{123, 0, 0, 0x00020000}};  // scale=2
  s21_decimal b = {{4, 0, 0, 0}};
  s21_decimal r;
  ck_assert_int_eq(s21_mul(a, b, &r), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&r), 2);
  ck_assert_uint_eq(r.bits[0], 492);
}
END_TEST

// Переполнение положительное и отрицательное
START_TEST(test_mul_overflow) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal r;
  ck_assert_int_eq(s21_mul(max, two, &r), S21_TOO_LARGE);

  s21_decimal neg_two = {{2, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_mul(max, neg_two, &r), S21_TOO_SMALL);
}
END_TEST

// Переполнение по scale (нормализация)
START_TEST(test_mul_scale_overflow) {
  // 1 * 0.1 с большим scale
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0x001C0000}};  // scale=28
  s21_decimal r;
  ck_assert_int_eq(s21_mul(a, b, &r), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&r), 28);
}
END_TEST

Suite* s21_mul_suite(void) {
  Suite* s = suite_create("mul");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_mul_basic_positive);
  tcase_add_test(tc, test_mul_by_zero);
  tcase_add_test(tc, test_mul_signs);
  tcase_add_test(tc, test_mul_carry_between_words);
  tcase_add_test(tc, test_mul_scale);
  tcase_add_test(tc, test_mul_overflow);
  tcase_add_test(tc, test_mul_scale_overflow);

  suite_add_tcase(s, tc);
  return s;
}
