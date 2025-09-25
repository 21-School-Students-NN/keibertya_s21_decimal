#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

// ---------- is_less ----------
START_TEST(test_is_less_positive) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  _set_sign(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  _set_sign(&b, 1);
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_less(b, a), 1);
}
END_TEST

START_TEST(test_is_less_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
  _set_sign(&b, 1);  // -0
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

// ---------- is_equal ----------
START_TEST(test_is_equal_zeros) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0x801A0000}};
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_basic) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{123, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_scale) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{123, 0, 0, 0}};
  _set_scale(&b, 2);
  _set_scale(&a, 2);
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

// ---------- is_not_equal ----------
START_TEST(test_is_not_equal_basic) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{6, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

// ---------- greater / less-or-equal ----------
START_TEST(test_is_greater_and_less_or_equal) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{60, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

// ---------- greater-or-equal ----------
START_TEST(test_is_greater_or_equal) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

Suite *s21_comparison_suite(void) {
  Suite *s = suite_create("comparison");
  TCase *tc = tcase_create("comparison_tc");

  tcase_add_test(tc, test_is_less_positive);
  tcase_add_test(tc, test_is_less_negative);
  tcase_add_test(tc, test_is_less_zero);

  tcase_add_test(tc, test_is_equal_zeros);
  tcase_add_test(tc, test_is_equal_basic);
  tcase_add_test(tc, test_is_equal_scale);

  tcase_add_test(tc, test_is_not_equal_basic);

  tcase_add_test(tc, test_is_greater_and_less_or_equal);
  tcase_add_test(tc, test_is_greater_or_equal);

  suite_add_tcase(s, tc);
  return s;
}
