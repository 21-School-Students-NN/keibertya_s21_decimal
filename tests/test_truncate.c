#include <check.h>
#include <stdint.h>
#include <string.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

START_TEST(test_truncate_positive_fraction) {
  s21_decimal in = make_decimal(12345u, 0u, 0u, 2u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(123u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_truncate_negative_fraction) {
  s21_decimal in = make_decimal(12345u, 0u, 0u, 2u, 1u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(123u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_truncate_positive_integer) {
  s21_decimal in = make_decimal(100u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_truncate_negative_integer) {
  s21_decimal in = make_decimal(100u, 0u, 0u, 0u, 1u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_truncate_positive_small_fraction_to_zero) {
  s21_decimal in = make_decimal(69u, 0u, 0u, 2u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_truncate_negative_small_fraction_to_negative_zero) {
  s21_decimal in = make_decimal(9u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  ck_assert_uint_eq(out.bits[0], 0u);
  ck_assert_uint_eq(out.bits[1], 0u);
  ck_assert_uint_eq(out.bits[2], 0u);
  ck_assert_uint_eq(_get_scale(&out), 0u);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_truncate_no_rounding_positive) {
  s21_decimal in = make_decimal(19u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_truncate_no_rounding_negative) {
  s21_decimal in = make_decimal(19u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_truncate_null_result) {
  s21_decimal in = make_decimal(123u, 0u, 0u, 2u, 0u);
  int res = s21_truncate(in, NULL);
  ck_assert_int_eq(res, S21_ERROR);
}
END_TEST

START_TEST(test_truncate_invalid_scale) {
  s21_decimal in = make_decimal(12345u, 0u, 0u, 29u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_ERROR);
}
END_TEST

START_TEST(test_truncate_scale_28) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_scale(&out), 0u);
}
END_TEST

START_TEST(test_truncate_maximum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
}
END_TEST

START_TEST(test_truncate_minimum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 1u);
  s21_decimal out;
  int res = s21_truncate(in, &out);
  ck_assert_int_eq(res, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
}
END_TEST

Suite *s21_truncate_suite(void) {
  Suite *s = suite_create("truncate");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_truncate_positive_fraction);
  tcase_add_test(tc_core, test_truncate_negative_fraction);
  tcase_add_test(tc_core, test_truncate_positive_integer);
  tcase_add_test(tc_core, test_truncate_negative_integer);
  tcase_add_test(tc_core, test_truncate_positive_small_fraction_to_zero);
  tcase_add_test(tc_core,
                 test_truncate_negative_small_fraction_to_negative_zero);
  tcase_add_test(tc_core, test_truncate_no_rounding_positive);
  tcase_add_test(tc_core, test_truncate_no_rounding_negative);
  tcase_add_test(tc_core, test_truncate_null_result);
  tcase_add_test(tc_core, test_truncate_invalid_scale);
  tcase_add_test(tc_core, test_truncate_scale_28);
  tcase_add_test(tc_core, test_truncate_maximum_no_scale);
  tcase_add_test(tc_core, test_truncate_minimum_no_scale);
  suite_add_tcase(s, tc_core);
  return s;
}
