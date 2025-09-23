#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/s21_decimal.h"
#include "../include/s21_suites.h"

START_TEST(test_from_int_to_decimal_positive) {
  s21_decimal dst;
  int result = s21_from_int_to_decimal(12345, &dst);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(dst.bits[0], 12345);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative) {
  s21_decimal dst;
  int result = s21_from_int_to_decimal(-12345, &dst);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(dst.bits[0], 12345);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq((dst.bits[3] >> 31) & 1, 1);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative_max) {
  s21_decimal dst;
  int result = s21_from_int_to_decimal(-2147483648, &dst);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(dst.bits[0], 2147483648);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq((dst.bits[3] >> 31) & 1, 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_simple) {
  s21_decimal dst;

  float fval = 12.3456f;
  int result = s21_from_float_to_decimal(fval, &dst);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(dst.bits[0], 1234560);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq((dst.bits[3] >> 16) & 0xff, 5);
  ck_assert_int_eq((dst.bits[3] >> 31) & 0xff, 0);
}
END_TEST

START_TEST(test_from_float_to_decimal_simple_negative) {
  s21_decimal dst;

  float fval = -1.23456e2;
  int result = s21_from_float_to_decimal(fval, &dst);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(dst.bits[0], 1234560);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq((dst.bits[3] >> 16) & 0xff, 4);
  ck_assert_int_eq((dst.bits[3] >> 31) & 0xff, 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_small_number) {
  s21_decimal dst;
  float fval = 1e-29f;
  int result = s21_from_float_to_decimal(fval, &dst);
  ck_assert_int_eq(result, 1);  // ERROR
}
END_TEST

START_TEST(test_from_decimal_to_int_no_fractional_part) {
  s21_decimal src;
  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  int iresult;
  int result = s21_from_decimal_to_int(src, &iresult);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_int_eq(iresult, 12345);
}
END_TEST

START_TEST(test_from_decimal_to_int_with_fractional_part) {
  s21_decimal src;

  src.bits[0] = 123456;  // 123456;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0x30000;  // scaling 10^-3
  int iresult;
  int result = s21_from_decimal_to_int(src, &iresult);
  ck_assert_int_eq(result, 0);     // SUCCESS
  ck_assert_int_eq(iresult, 123);  // int value
}
END_TEST

START_TEST(test_from_decimal_to_int_with_fractional_part_negative) {
  s21_decimal src;

  src.bits[0] = 123456;  // 123456;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0x80030000;  // scaling 10^-3 and negative
  int iresult;
  int result = s21_from_decimal_to_int(src, &iresult);
  ck_assert_int_eq(result, 0);      // SUCCESS
  ck_assert_int_eq(iresult, -123);  // int value
}
END_TEST

START_TEST(test_from_decimal_to_int_large_val) {
  s21_decimal src;
  src.bits[0] = 123456;
  src.bits[1] = 0;
  src.bits[2] = 1;
  src.bits[3] = (3 << 16);
  int iresult;
  int result = s21_from_decimal_to_int(src, &iresult);
  ck_assert_int_eq(result, 1);  // ERROR
}
END_TEST

START_TEST(test_from_decimal_to_float_whole_number) {
  s21_decimal src;
  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  float fresult;
  int result = s21_from_decimal_to_float(src, &fresult);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_double_eq_tol(
      fresult, 12345.0f,
      FLT_EPSILON);  // Epsilon count some incorrection in value
}
END_TEST

START_TEST(test_from_decimal_to_float_fractional_number) {
  s21_decimal src;
  src.bits[0] = 123456;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = (3 << 16);
  float fresult;
  int result = s21_from_decimal_to_float(src, &fresult);
  ck_assert_int_eq(result, 0);  // SUCCESS
  ck_assert_double_eq_tol(fresult, 123.456f, FLT_EPSILON);
}
END_TEST

START_TEST(test_from_decimal_to_float_rounding) {
  s21_decimal src;
  src.bits[0] = 123456789;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0x80080000u;
  float fresult;
  int result = s21_from_decimal_to_float(src, &fresult);
  ck_assert_int_eq(result, 0);  // SUCCESs
  ck_assert_double_eq_tol(fresult, -1.23456789f, FLT_EPSILON);
}
END_TEST

#ifdef ENABLE_EXTENDED_TESTS
#include "../include/s21_helpers.h"

START_TEST(test_from_float_zero) {
  float n = 0.0f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(n, &result), 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(_get_scale(&result), 0);
}
END_TEST

START_TEST(test_from_float_negative_zero) {
  float n = -0.0f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(n, &result), 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_from_float_positive_integer) {
  float n = +123.0f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(n, &result), 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(_get_scale(&result), 0);
}
END_TEST

START_TEST(test_from_float_negative_integer) {
  float n = -456.0f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(n, &result), 0);
  ck_assert_int_eq(result.bits[0], 456);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_from_float_exact_scale_check) {
  float n = 0.000123f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(n, &result), 0);
  ck_assert_int_eq(_get_scale(&result), 6);
  ck_assert_int_eq(result.bits[0], 123);
}
END_TEST

START_TEST(test_from_float_max_exact_boundary) {
  float n = 7.9228162514264337593543950335e28f;
  s21_decimal result;
  int ret = s21_from_float_to_decimal(n, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST
#endif

Suite *s21_converters_suite(void) {
  Suite *suite = suite_create("converters");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_from_int_to_decimal_positive);
  tcase_add_test(tc_core, test_from_int_to_decimal_negative);
  tcase_add_test(tc_core, test_from_int_to_decimal_negative_max);
  tcase_add_test(tc_core, test_from_float_to_decimal_simple);
  tcase_add_test(tc_core, test_from_float_to_decimal_simple_negative);
  tcase_add_test(tc_core, test_from_float_to_decimal_small_number);
  tcase_add_test(tc_core, test_from_decimal_to_int_no_fractional_part);
  tcase_add_test(tc_core,
                 test_from_decimal_to_int_with_fractional_part_negative);
  tcase_add_test(tc_core, test_from_decimal_to_int_with_fractional_part);
  tcase_add_test(tc_core, test_from_decimal_to_int_large_val);
  tcase_add_test(tc_core, test_from_decimal_to_float_whole_number);
  tcase_add_test(tc_core, test_from_decimal_to_float_fractional_number);
  tcase_add_test(tc_core, test_from_decimal_to_float_rounding);

#ifdef ENABLE_EXTENDED_TESTS
  TCase *tc_extended = tcase_create("extended");

  tcase_add_test(tc_extended, test_from_float_zero);
  tcase_add_test(tc_extended, test_from_float_negative_zero);
  tcase_add_test(tc_extended, test_from_float_positive_integer);
  tcase_add_test(tc_extended, test_from_float_negative_integer);
  tcase_add_test(tc_extended, test_from_float_exact_scale_check);
  tcase_add_test(tc_extended, test_from_float_max_exact_boundary);

  suite_add_tcase(suite, tc_extended);
#endif

  suite_add_tcase(suite, tc_core);
  return suite;
}