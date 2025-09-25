// tests/test_s21_round.c
#include <check.h>
#include <stdint.h>
#include <string.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

START_TEST(test_round_positive_less_half) {
  s21_decimal in = make_decimal(14u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_round_positive_more_half) {
  s21_decimal in = make_decimal(16u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_positive_tie_15) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_positive_tie_25_half_up) {
  s21_decimal in = make_decimal(25u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_negative_tie_15) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_round_negative_tie_25_half_up) {
  s21_decimal in = make_decimal(25u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_positive_zero_half_up) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_round_negative_zero_half_up) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
  ck_assert_uint_eq(_get_sign(&out), 1u);
}
END_TEST

START_TEST(test_round_scale_28_to_zero) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_round_scale_zero_noop) {
  s21_decimal in = make_decimal(123u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(out, in), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_round_positive_large_fraction) {
  s21_decimal in = make_decimal(1236u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(124u, 0u, 0u, 0u, 0u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_negative_large_fraction) {
  s21_decimal in = make_decimal(1236u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(124u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
}
END_TEST

START_TEST(test_round_null_result) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  int rc = s21_round(in, NULL);
  ck_assert_int_eq(rc, S21_ERROR);
}
END_TEST

START_TEST(test_round_invalid_scale) {
  s21_decimal in = make_decimal(12345u, 0u, 0u, 29u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_ERROR);
}
END_TEST

START_TEST(test_round_large_high_bits_sanity) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0x12345678u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  ck_assert_int_eq(_get_scale(&out), 0);
  ck_assert_uint_eq(_get_sign(&out), 0u);
}
END_TEST

START_TEST(test_round_negative_less_half) {
  s21_decimal in = make_decimal(14u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_round_negative_equal_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

START_TEST(test_round_negative_greater_half) {
  s21_decimal in = make_decimal(16u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  ck_assert_int_eq(s21_is_equal(out, expected), 1);
  ck_assert_int_eq(_get_scale(&out), 0);
}
END_TEST

#ifdef ENABLE_EXTENDED_TESTS

#include "../include/s21_helpers.h"

START_TEST(test_round_down_half) {
  s21_decimal val = {{5, 0, 0, 0}};  // 0.5
  _set_scale(&val, 1);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(_get_scale(&res), 0);
}
END_TEST

START_TEST(test_round_negative_half) {
  s21_decimal val = {{5, 0, 0, 0}};  // -0.5
  _set_scale(&val, 1);
  _set_sign(&val, 1);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(_get_sign(&res), 1);
}
END_TEST

START_TEST(test_round_negative_up) {
  s21_decimal val = {{25, 0, 0, 0}};  // -2.5
  _set_scale(&val, 1);
  _set_sign(&val, 1);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(_get_sign(&res), 1);
}
END_TEST
#endif

Suite *s21_round_suite(void) {
  Suite *s = suite_create("round");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_round_positive_less_half);
  tcase_add_test(tc_core, test_round_positive_more_half);
  tcase_add_test(tc_core, test_round_positive_tie_15);
  tcase_add_test(tc_core, test_round_positive_tie_25_half_up);
  tcase_add_test(tc_core, test_round_negative_tie_15);
  tcase_add_test(tc_core, test_round_negative_tie_25_half_up);
  tcase_add_test(tc_core, test_round_positive_zero_half_up);
  tcase_add_test(tc_core, test_round_negative_zero_half_up);
  tcase_add_test(tc_core, test_round_scale_28_to_zero);
  tcase_add_test(tc_core, test_round_scale_zero_noop);
  tcase_add_test(tc_core, test_round_positive_large_fraction);
  tcase_add_test(tc_core, test_round_negative_large_fraction);
  tcase_add_test(tc_core, test_round_null_result);
  tcase_add_test(tc_core, test_round_invalid_scale);
  tcase_add_test(tc_core, test_round_large_high_bits_sanity);
  tcase_add_test(tc_core, test_round_negative_less_half);
  tcase_add_test(tc_core, test_round_negative_equal_half);
  tcase_add_test(tc_core, test_round_negative_greater_half);

#ifdef ENABLE_EXTENDED_TESTS
  TCase *tc_extended = tcase_create("extended");

  tcase_add_test(tc_extended, test_round_down_half);
  tcase_add_test(tc_extended, test_round_negative_half);
  tcase_add_test(tc_extended, test_round_negative_up);

  suite_add_tcase(s, tc_extended);
#endif

  suite_add_tcase(s, tc_core);
  return s;
}