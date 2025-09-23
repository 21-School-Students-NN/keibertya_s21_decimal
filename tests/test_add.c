#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_add_positive_no_overflow) {
  s21_decimal a = {{123, 0, 0, 0}};
  s21_decimal b = {{456, 0, 0, 0}};
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on basic 123 + 456");

  ck_assert_uint_eq(result.bits[0], 579);
  ck_assert_uint_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_add_carry_into_mid) {
  s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add with carry 0xFFFFFFFF + 1");

  ck_assert_uint_eq(result.bits[0], 0);
  ck_assert_uint_eq(result.bits[1], 1);  // Carry propagated
  ck_assert_uint_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_add_negative) {
  s21_decimal a = {{100, 0, 0, 0x80000000}};  // -100
  s21_decimal b = {{50, 0, 0, 0x80000000}};   // -50
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add negative value (-100) + (-50)");

  ck_assert_uint_eq(result.bits[0], 150);
  ck_assert_uint_eq(_get_sign(&result), 1);  // Negative
}
END_TEST

START_TEST(test_add_different) {
  s21_decimal a = {{100, 0, 0, 0x80000000}};  // -100
  s21_decimal b = {{50, 0, 0, 0}};            // 50
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add different value (-100) + 50");

  ck_assert_uint_eq(result.bits[0], 50);
  ck_assert_uint_eq(_get_sign(&result), 1);  // Negative
}
END_TEST

START_TEST(test_add_max_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // 2^96 - 1
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_TOO_LARGE,
                "s21_add doesn't return error on positive overflow");
}
END_TEST

START_TEST(test_add_min_overflow) {
  s21_decimal a = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // -2^96 - 1
  s21_decimal b = {{1, 0, 0, 0x80000000}};
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_TOO_SMALL,
                "s21_add doesn't return error on negative overflow");
}
END_TEST

START_TEST(test_add_same_cale) {
  s21_decimal a = {{123456, 0, 0, 0x00040000}};
  s21_decimal b = {{216543, 0, 0, 0x00040000}};
  s21_decimal result;

  ck_assert_msg(
      s21_add(a, b, &result) == S21_SUCCESS,
      "s21_add returned error on add with same scale 12.3456 + 21.6543");

  ck_assert_uint_eq(result.bits[0], 123456 + 216543);
  ck_assert_uint_eq(_get_sign(&result), 0);
  ck_assert_uint_eq(_get_scale(&result), 4);
}
END_TEST

START_TEST(test_add_different_scales) {
  s21_decimal a = {{1234, 0, 0, 0x00020000}};  // 12.34
  s21_decimal b = {{567, 0, 0, 0x00010000}};   // 56.7
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add with diff scale 12.34 + 56.7");

  ck_assert_uint_eq(result.bits[0], 1234 + 5670);  // Scaled to 10^-2
  ck_assert_uint_eq(_get_scale(&result), 2);
}
END_TEST

#ifdef ENABLE_EXTENDED_TESTS

#endif

START_TEST(test_add_regular_rounding) {
  s21_decimal a = {
      {0xFFFFFFFF - 1, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};  // 2^96 - 2
  s21_decimal b = {{61, 0, 0, 0x00020000}};                   // 0.51
  s21_decimal result;

  ck_assert_msg(
      s21_add(a, b, &result) == S21_SUCCESS,
      "s21_add returned error on add with diff scale 2^96 - 2 + 0.61");

  for (int i = 0; i < 2; ++i) ck_assert_uint_eq(result.bits[i], 0xFFFFFFFF);
  ck_assert_uint_eq(_get_scale(&result), 0);
}
END_TEST

#ifdef ENABLE_EXTENDED_TESTS

START_TEST(test_add_edge_case_1) {
  // -0.5
  s21_decimal value_1 = {{0x5, 0x0, 0x0, 0x80010000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  // 79228162514264337593543950334
  s21_decimal expected = {{0xfffffffe, 0xffffffff, 0xffffffff, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_edge_case_2) {
  // -79228162514264337593543950334
  s21_decimal value_1 = {{0xfffffffe, 0xffffffff, 0xffffffff, 0x80000000}};
  // 0.5000000000000000000000000001
  s21_decimal value_2 = {{0x88000001, 0x1f128130, 0x1027e72f, 0x1c0000}};
  s21_decimal result = {{0}};
  // -79228162514264337593543950333
  s21_decimal expected = {{0xfffffffd, 0xffffffff, 0xffffffff, 0x80000000}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_edge_case_3) {
  // 0.5
  s21_decimal value_1 = {{0x5, 0x0, 0x0, 0x10000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_edge_case_4) {
  s21_decimal a = {
      {0xFFFFFFFF - 1, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};  // 2^96 - 2
  s21_decimal b = {{51, 0, 0, 0x00020000}};                   // 0.51
  s21_decimal result;

  ck_assert_msg(
      s21_add(a, b, &result) == S21_SUCCESS,
      "s21_add returned error on add with diff scale 2^96 - 2 + 0.51");

  for (int i = 0; i < 2; ++i) ck_assert_uint_eq(result.bits[i], 0xFFFFFFFF);
  ck_assert_uint_eq(_get_scale(&result), 0);
}
END_TEST
#endif

Suite* s21_add_suite() {
  Suite* ps = suite_create("add");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_add_positive_no_overflow);
  tcase_add_test(tc, test_add_carry_into_mid);
  tcase_add_test(tc, test_add_negative);
  tcase_add_test(tc, test_add_different);
  tcase_add_test(tc, test_add_max_overflow);
  tcase_add_test(tc, test_add_min_overflow);
  tcase_add_test(tc, test_add_same_cale);
  tcase_add_test(tc, test_add_different_scales);
  tcase_add_test(tc, test_add_regular_rounding);

#ifdef ENABLE_EXTENDED_TESTS
  TCase* tc_extended = tcase_create("extended");

  tcase_add_test(tc_extended, test_add_edge_case_1);
  tcase_add_test(tc_extended, test_add_edge_case_2);
  tcase_add_test(tc_extended, test_add_edge_case_3);
  tcase_add_test(tc_extended, test_add_edge_case_4);

  suite_add_tcase(ps, tc_extended);
#endif

  suite_add_tcase(ps, tc);
  return ps;
}