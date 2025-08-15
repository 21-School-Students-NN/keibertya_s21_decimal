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

START_TEST(test_add_bank_rounding) {
  s21_decimal a = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};  // 2^96 - 2
  s21_decimal b = {{51, 0, 0, 0x00020000}};   // 0.51
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add with diff scale 2^96 - 2 + 0.51");

  for (int i = 0; i < 2; ++i)
    ck_assert_uint_eq(result.bits[i], 0xFFFFFFFF);
  ck_assert_uint_eq(_get_scale(&result), 0);
}
END_TEST

/** 
 * test-case where we add to real big number small one, with different scale
 * programm should use `bancking rounding` for the answer
 * 
 * good if we can find way to create such use-case: 
 * 2^96 - 1 (full up mantisa) with scale 10 + some number = full up mantisa with scale 8
 */
#if 0
START_TEST(test_add_bank_rounding_1) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000A0000}};  // 2^96 - 1 / 10^10
  s21_decimal b = {{51, 0, 0, 0x000A0000}};   // 0x62 ffffffff ffffffff ffffff9d
  s21_decimal result;

  ck_assert_msg(s21_add(a, b, &result) == S21_SUCCESS,
                "s21_add returned error on add with diff scale 2^96 - 2 + 0.51");

  for (int i = 0; i < 2; ++i)
    ck_assert_uint_eq(result.bits[i], 0xFFFFFFFF);
  ck_assert_uint_eq(_get_scale(&result), 8);
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
  tcase_add_test(tc, test_add_bank_rounding);

  suite_add_tcase(ps, tc);
  return ps;
}