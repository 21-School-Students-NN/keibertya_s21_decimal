#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_sub_zero) {
  s21_decimal number = {{0xDEAFBEEF, 0xCAFEBABE, 0x01010101, 0}};
  s21_decimal result;
  s21_decimal zero = {{0, 0, 0, 0}};

  s21_sub(number, zero, &result);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));

  _set_sign(&zero, 1);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));

  _set_scale(&zero, 11);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_sub_small_decimal) {
  s21_decimal minuend = {{444, 0, 0, 0}};
  s21_decimal subtrahend = {{44, 0, 0, 0}};
  s21_decimal difference;
  s21_decimal expected_difference = {{400, 0, 0, 0}};

  int res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction 444 - 44", res);

  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));
}
END_TEST

Suite *s21_sub_suite() {
  Suite *ps = suite_create("sub");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_zero);

  suite_add_tcase(ps, tc);
  return ps;
}