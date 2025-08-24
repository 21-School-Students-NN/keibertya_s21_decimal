// tests/test_s21_round.c
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

START_TEST(test_round_positive_less_half) {
  s21_decimal in = make_decimal(14u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_round_positive_more_half) {
  s21_decimal in = make_decimal(16u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_round_positive_tie_15) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_round_positive_tie_25_half_up) {
  s21_decimal in = make_decimal(25u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(3u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_round_negative_tie_15) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 1u);
}
END_TEST

START_TEST(test_round_negative_tie_25_half_up) {
  s21_decimal in = make_decimal(25u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(3u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_round_positive_zero_half_up) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  ck_assert_uint_eq(get_sign(&out), 0u);
}
END_TEST

START_TEST(test_round_negative_zero_half_up) {
  s21_decimal in = make_decimal(5u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 1u);
}
END_TEST

START_TEST(test_round_scale_28_to_zero) {
  s21_decimal in = make_decimal(1u, 0u, 0u, 28u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(0u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_round_scale_zero_noop) {
  s21_decimal in = make_decimal(123u, 0u, 0u, 0u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  assert_decimal_eq(out, in);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_round_positive_large_fraction) {
  s21_decimal in = make_decimal(1236u, 0u, 0u, 1u, 0u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(124u, 0u, 0u, 0u, 0u);
  assert_decimal_eq(out, expected);
}
END_TEST

START_TEST(test_round_negative_large_fraction) {
  s21_decimal in = make_decimal(1236u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(124u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
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
  assert_scale_zero(&out);
  ck_assert_uint_eq(get_sign(&out), 0u);
  ck_assert_uint_le(out.bits[2], in.bits[2]);
}
END_TEST

START_TEST(test_round_negative_less_half) {
  s21_decimal in = make_decimal(14u, 0u, 0u, 1u, 2u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(1u, 0u, 0u, 0u, 2u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_round_negative_equal_half) {
  s21_decimal in = make_decimal(15u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

START_TEST(test_round_negative_greater_half) {
  s21_decimal in = make_decimal(16u, 0u, 0u, 1u, 1u);
  s21_decimal out;
  int rc = s21_round(in, &out);
  ck_assert_int_eq(rc, S21_SUCCESS);
  s21_decimal expected = make_decimal(2u, 0u, 0u, 0u, 1u);
  assert_decimal_eq(out, expected);
  assert_scale_zero(&out);
}
END_TEST

Suite *s21_round_suite(void) {
  Suite *s = suite_create("s21_round");
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
  suite_add_tcase(s, tc_core);
  return s;
}