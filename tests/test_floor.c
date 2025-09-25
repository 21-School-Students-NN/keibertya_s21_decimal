#include <check.h>
#include <stdint.h>
#include <string.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_floor_positive_fraction) {
  s21_decimal in = make_decimal(145u, 0u, 0u, 2u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_positive_integer) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_positive_small_fraction_to_zero) {
  s21_decimal in = make_decimal(9u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal in = make_decimal(145u, 0u, 0u, 2u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_floor_negative_integer) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 0u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_negative_small_fraction_to_minus_one) {
  s21_decimal in = make_decimal(9u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_zero_positive) {
  s21_decimal in = make_decimal(0u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_negative_zero_preserve_sign) {
  s21_decimal in = make_decimal(0u, 0u, 0u, 0u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_uint_eq(out.bits[0], 0u);
  ck_assert_uint_eq(out.bits[1], 0u);
  ck_assert_uint_eq(out.bits[2], 0u);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_floor_positive_tiny_scale28_to_zero) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_negative_tiny_scale28_to_minus_one) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_floor_null_result) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  int rc = s21_floor(in, NULL);
  ck_assert_int_eq(rc, S21_ERROR);
}
END_TEST

START_TEST(test_floor_invalid_scale) {
  s21_decimal in = make_decimal(12345u, 0u, 0u, 29u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_ERROR);
}
END_TEST

START_TEST(test_floor_large_high_bits_sanity) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0x12345677u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  s21_decimal expected = in;
  s21_truncate(expected, &expected);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_floor_maximum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
}
END_TEST

START_TEST(test_floor_minimum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
}
END_TEST

START_TEST(test_floor_negative_exact_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_floor_positive_exact_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

Suite *s21_floor_suite(void) {
  Suite *s = suite_create("floor");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_floor_positive_fraction);
  tcase_add_test(tc_core, test_floor_positive_integer);
  tcase_add_test(tc_core, test_floor_positive_small_fraction_to_zero);
  tcase_add_test(tc_core, test_floor_negative_fraction);
  tcase_add_test(tc_core, test_floor_negative_integer);
  tcase_add_test(tc_core, test_floor_negative_small_fraction_to_minus_one);
  tcase_add_test(tc_core, test_floor_zero_positive);
  tcase_add_test(tc_core, test_floor_negative_zero_preserve_sign);
  tcase_add_test(tc_core, test_floor_positive_tiny_scale28_to_zero);
  tcase_add_test(tc_core, test_floor_negative_tiny_scale28_to_minus_one);
  tcase_add_test(tc_core, test_floor_null_result);
  tcase_add_test(tc_core, test_floor_invalid_scale);
  tcase_add_test(tc_core, test_floor_large_high_bits_sanity);
  tcase_add_test(tc_core, test_floor_maximum_no_scale);
  tcase_add_test(tc_core, test_floor_minimum_no_scale);
  tcase_add_test(tc_core, test_floor_negative_exact_half);
  tcase_add_test(tc_core, test_floor_positive_exact_half);

  suite_add_tcase(s, tc_core);
  return s;
}