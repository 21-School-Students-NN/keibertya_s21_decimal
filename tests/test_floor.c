#include <check.h>
#include <stdint.h>
#include <string.h>

#include "../include/s21_decimal.h"

static s21_decimal make_decimal(uint32_t low, uint32_t mid, uint32_t high,
                                uint32_t scale, uint32_t sign) {
  s21_decimal d;
  d.bits[0] = low;
  d.bits[1] = mid;
  d.bits[2] = high;
  d.bits[3] = ((scale & 0xFFu) << 16) | ((sign & 0x1u) << 31);
  return d;
}

static void assert_decimal_eq(s21_decimal a, s21_decimal b) {
  ck_assert_uint_eq(a.bits[0], b.bits[0]);
  ck_assert_uint_eq(a.bits[1], b.bits[1]);
  ck_assert_uint_eq(a.bits[2], b.bits[2]);
  ck_assert_uint_eq(a.bits[3], b.bits[3]);
}

static void assert_scale_zero(const s21_decimal *d) {
  uint32_t scale = (d->bits[3] >> 16) & 0xFFu;
  ck_assert_uint_eq(scale, 0u);
}

static uint32_t get_sign(const s21_decimal *d) {
  return (d->bits[3] >> 31) & 0x1u;
}

START_TEST(test_floor_positive_fraction) {
  s21_decimal in = make_decimal(145u, 0u, 0u, 2u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_positive_integer) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_positive_small_fraction_to_zero) {
  s21_decimal in = make_decimal(9u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal in = make_decimal(145u, 0u, 0u, 2u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 1u);
}
END_TEST

START_TEST(test_floor_negative_integer) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 0u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_negative_small_fraction_to_minus_one) {
  s21_decimal in = make_decimal(9u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_zero_positive) {
  s21_decimal in = make_decimal(0u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
  assert_scale_zero(&out);
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
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 1u);
}
END_TEST

START_TEST(test_floor_positive_tiny_scale28_to_zero) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_floor_negative_tiny_scale28_to_minus_one) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
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
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0x12345678u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 0u);
  ck_assert_uint_le(out.bits[2], in.bits[2]);
}
END_TEST

START_TEST(test_floor_maximum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
}
END_TEST

START_TEST(test_floor_minimum_no_scale) {
  s21_decimal in = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
}
END_TEST

START_TEST(test_floor_negative_exact_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_floor_positive_exact_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_floor_negative_large_fraction_to_minus145) {
  s21_decimal in = make_decimal(14500u, 0u, 0u, 2u, 1u);
  s21_decimal out;
  int rc = s21_floor(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(145u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 1u);
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
  tcase_add_test(tc_core, test_floor_negative_large_fraction_to_minus145);

  suite_add_tcase(s, tc_core);
  return s;
}