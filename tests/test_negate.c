#include "../include/s21_decimal.h"
#include "../include/s21_suites.h"

START_TEST(test_negate_positive) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[3] >> 31, 1);
  ck_assert_int_eq(res.bits[0], 123);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal a = {{123, 0, 0, 0x80000000}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[3] >> 31, 0);
  ck_assert_int_eq(res.bits[0], 123);
}
END_TEST

START_TEST(test_negate_zero_positive) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_negate_zero_negative) {
  s21_decimal a = {{0, 0, 0, 0x80000000}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3] >> 31, 0);
}
END_TEST

START_TEST(test_negate_null_result) {
  s21_decimal a = {{123, 0, 0, 0}};
  ck_assert_int_eq(s21_negate(a, NULL), 1);
}
END_TEST

START_TEST(test_negate_scale_preserved) {
  s21_decimal a = {{1000, 0, 0, 0}};
  a.bits[3] = (2 << 16);
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq((res.bits[3] >> 16) & 0xFF, 2);
  ck_assert_int_eq(res.bits[3] >> 31, 1);
}
END_TEST

START_TEST(test_negate_max_value) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[3] >> 31, 1);
}
END_TEST

START_TEST(test_negate_min_value) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal res;
  ck_assert_int_eq(s21_negate(a, &res), 0);
  ck_assert_int_eq(res.bits[3] >> 31, 0);
}
END_TEST

Suite *s21_negate_suite(void) {
  Suite *s = suite_create("negate");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_negate_positive);
  tcase_add_test(tc, test_negate_negative);
  tcase_add_test(tc, test_negate_zero_positive);
  tcase_add_test(tc, test_negate_zero_negative);
  tcase_add_test(tc, test_negate_null_result);
  tcase_add_test(tc, test_negate_scale_preserved);
  tcase_add_test(tc, test_negate_max_value);
  tcase_add_test(tc, test_negate_min_value);

  suite_add_tcase(s, tc);
  return s;
}
