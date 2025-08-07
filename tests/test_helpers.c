#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

// =============================================================================
// COMPREHENSIVE TEST CASES
// =============================================================================

START_TEST(test_initialization) {
  s21_decimal dec1, dec2;

  // Test _init_decimal_zero
  _init_decimal_zero(&dec1);
  ck_assert_uint_eq(dec1.bits[0], 0);
  ck_assert_uint_eq(dec1.bits[1], 0);
  ck_assert_uint_eq(dec1.bits[2], 0);
  ck_assert_uint_eq(dec1.bits[3], 0);

  // Verify default values after initialization
  ck_assert_uint_eq(_get_scale(&dec1), 0);
  ck_assert_uint_eq(_get_sign(&dec1), 0);

  // Test that initialization doesn't affect other decimals
  dec2.bits[0] = 0x12345678;
  dec2.bits[1] = 0x87654321;
  dec2.bits[2] = 0xDEADBEEF;
  dec2.bits[3] = 0xCAFEBABE;

  _init_decimal_zero(&dec1);
  ck_assert_uint_eq(dec1.bits[0], 0);
  ck_assert_uint_ne(dec2.bits[0], 0);  // dec2 should be unchanged
}
END_TEST

START_TEST(test_scale_operations) {
  s21_decimal dec;
  _init_decimal_zero(&dec);

  // Test _get_scale with default zero
  ck_assert_uint_eq(_get_scale(&dec), 0);

  // Test _set_scale with valid values
  ck_assert_int_eq(_set_scale(&dec, 5), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&dec), 5);

  ck_assert_int_eq(_set_scale(&dec, 28), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&dec), 28);

  // Test _set_scale with invalid values
  ck_assert_int_eq(_set_scale(&dec, 29), S21_ERROR);
  ck_assert_int_eq(_set_scale(&dec, 255), S21_ERROR);
}
END_TEST

START_TEST(test_sign_operations) {
  s21_decimal dec;
  _init_decimal_zero(&dec);

  // Test _get_sign with default zero
  ck_assert_uint_eq(_get_sign(&dec), 0);

  // Test _set_sign with valid values
  ck_assert_int_eq(_set_sign(&dec, 1), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&dec), 1);

  ck_assert_int_eq(_set_sign(&dec, 0), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&dec), 0);

  // Test _set_sign with invalid values
  ck_assert_int_eq(_set_sign(&dec, 2), S21_ERROR);
  ck_assert_int_eq(_set_sign(&dec, 255), S21_ERROR);
}
END_TEST

START_TEST(test_scale_and_sign_interaction) {
  s21_decimal dec;
  _init_decimal_zero(&dec);

  // Set scale and verify it doesn't affect sign
  ck_assert_int_eq(_set_scale(&dec, 15), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&dec), 15);
  ck_assert_uint_eq(_get_sign(&dec), 0);

  // Set sign and verify it doesn't affect scale
  ck_assert_int_eq(_set_sign(&dec, 1), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&dec), 1);
  ck_assert_uint_eq(_get_scale(&dec), 15);

  // Change scale while preserving sign
  ck_assert_int_eq(_set_scale(&dec, 20), S21_SUCCESS);
  ck_assert_uint_eq(_get_scale(&dec), 20);
  ck_assert_uint_eq(_get_sign(&dec), 1);

  // Change sign while preserving scale
  ck_assert_int_eq(_set_sign(&dec, 0), S21_SUCCESS);
  ck_assert_uint_eq(_get_sign(&dec), 0);
  ck_assert_uint_eq(_get_scale(&dec), 20);
}
END_TEST

START_TEST(test_edge_cases_and_boundaries) {
  s21_decimal dec;
  _init_decimal_zero(&dec);

  // Test all valid scale values
  for (int i = 0; i <= 28; i++) {
    ck_assert_int_eq(_set_scale(&dec, i), S21_SUCCESS);
    ck_assert_uint_eq(_get_scale(&dec), i);
  }

  // Test with maximum values in bits
  dec.bits[0] = 0xFFFFFFFF;
  dec.bits[1] = 0xFFFFFFFF;
  dec.bits[2] = 0xFFFFFFFF;
  dec.bits[3] = 0x7FFFFFFF;  // All bits set except sign

  ck_assert_uint_eq(_get_scale(&dec), 31);
  ck_assert_uint_eq(_get_sign(&dec), 0);

  // Test with sign bit set
  dec.bits[3] = 0xFFFFFFFF;
  ck_assert_uint_eq(_get_sign(&dec), 1);

  // Test scale extraction with sign
  dec.bits[3] = (1U << 31) | (25 << 16);
  ck_assert_uint_eq(_get_scale(&dec), 25);
  ck_assert_uint_eq(_get_sign(&dec), 1);
}
END_TEST

// =============================================================================
// SUITE CREATION
// =============================================================================

Suite *s21_helpers_suite(void) {
  Suite *s = suite_create("helpers");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_scale_operations);
  tcase_add_test(tc, test_sign_operations);
  tcase_add_test(tc, test_scale_and_sign_interaction);
  tcase_add_test(tc, test_edge_cases_and_boundaries);
  tcase_add_test(tc, test_initialization);

  suite_add_tcase(s, tc);
  return s;
}
