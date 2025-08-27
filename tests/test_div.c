#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_div_bu_null) {
  s21_decimal number = {{0xDEADBEEF, 0xCAFEBABE, 0x10101010, 0}};
  s21_decimal null = {{0, 0, 0, 0}};

  s21_decimal result;

  ck_assert_msg(s21_div(number, null, &result) == S21_DIV_BY_ZERO,
                "s21_div doesn't return error on div by zero (positive)");

  _set_sign(&null, 1);
  ck_assert_msg(s21_div(number, null, &result) == S21_DIV_BY_ZERO,
                "s21_div doesn't return error on div by zero (negative)");

  _set_sign(&null, 17);
  ck_assert_msg(
      s21_div(number, null, &result) == S21_DIV_BY_ZERO,
      "s21_div doesn't return error on div by zero (negative & scaled)");
}
END_TEST

Suite *s21_div_suite() {
  Suite *ps = suite_create("div");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_div_bu_null);

  suite_add_tcase(ps, tc);
  return ps;
}