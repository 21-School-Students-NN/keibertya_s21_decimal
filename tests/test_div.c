#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_div_by_null) {
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

START_TEST(test_div_zero_by_number) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal number = {{12345, 0, 0, 0}};
  _set_scale(&number, 2);

  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  ck_assert_msg(s21_div(zero, number, &result) == S21_SUCCESS,
                "s21_div failed on zero divided by number");
  ck_assert_msg(_is_equal(result, expected), "0 / number should be 0");
}
END_TEST

START_TEST(test_div_one_by_one) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{1, 0, 0, 0}};

  ck_assert_msg(s21_div(one, one, &result) == S21_SUCCESS,
                "s21_div failed on 1 / 1");
  ck_assert_msg(_is_equal(result, expected), "1 / 1 should be 1");
}
END_TEST

START_TEST(test_div_ten_by_two) {
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{5, 0, 0, 0}};

  ck_assert_msg(s21_div(ten, two, &result) == S21_SUCCESS,
                "s21_div failed on 10 / 2");
  ck_assert_msg(_is_equal(result, expected), "10 / 2 should be 5");
}
END_TEST

START_TEST(test_div_with_different_scales) {
  s21_decimal num1 = {{100, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  _set_scale(&num1, 1);  // 10.0
  _set_scale(&num2, 0);  // 10

  s21_decimal result;
  s21_decimal expected = {{10, 0, 0, 0}};
  _set_scale(&expected, 1);  // 1.0
  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed on numbers with different scales");
  ck_assert_msg(_is_equal(result, expected), "10.0 / 10 should be 1.0");
}
END_TEST

START_TEST(test_div_with_negative) {
  s21_decimal num1 = {{100, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  _set_sign(&num1, 1);  // -100

  s21_decimal result;
  s21_decimal expected = {{10, 0, 0, 0}};
  _set_sign(&expected, 1);  // -10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with negative dividend");
  ck_assert_msg(_is_equal(result, expected), "-100 / 10 should be -10");

  _set_sign(&num1, 0);      // 100
  _set_sign(&num2, 1);      // -10
  _set_sign(&expected, 1);  // -10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with negative divisor");
  ck_assert_msg(_is_equal(result, expected), "100 / -10 should be -10");

  _set_sign(&num1, 1);      // -100
  _set_sign(&expected, 0);  // 10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with both negative");
  ck_assert_msg(_is_equal(result, expected), "-100 / -10 should be 10");
}
END_TEST

START_TEST(test_div_large_numbers) {
  s21_decimal num1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // max 96-bit
  s21_decimal num2 = {{1, 0, 0, 0}};

  s21_decimal result;
  s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with large numbers");
  ck_assert_msg(_is_equal(result, expected),
                "Max value / 1 should be max value");
}
END_TEST

START_TEST(test_div_rounding) {
  s21_decimal num1 = {{10, 0, 0, 0}};
  s21_decimal num2 = {{4, 0, 0, 0}};

  s21_decimal result;
  s21_decimal expected = {{25, 0, 0, 0}};
  _set_scale(&expected, 1);  // 2.5
  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with rounding test");
  ck_assert_msg(_is_equal(result, expected), "10 / 4 should be 2.5");
}
END_TEST

START_TEST(test_div_periodic_fraction) {
  s21_decimal num1 = {{1, 0, 0, 0}};
  s21_decimal num2 = {{3, 0, 0, 0}};

  s21_decimal result;

  // 1/3 = 0.3333333333333333333333333333 -> to hex: AC544CA14B700CB05555555
  s21_decimal expected = {{0x05555555, 0x14B700CB, 0x0AC544CA, 0}};
  _set_scale(&expected, 28);  // 28 знаков после запятой

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed on 1 / 3");
  ck_assert_msg(_is_equal(result, expected),
                "1 / 3 should be 0.3333333333333333333333333333");
}
END_TEST

START_TEST(test_div_complex_periodic) {
  s21_decimal num1 = {{1, 0, 0, 0}};
  s21_decimal num2 = {{13, 0, 0, 0}};

  s21_decimal result;
  // 1/13 = 0.0769230769230769230769230769 -> to hex: 0x27C4AF38EA062A5013B13B1
  s21_decimal expected = {{0x013B13B1, 0x8EA062A5, 0x27C4AF3, 0}};
  _set_scale(&expected, 28);

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed on complex periodic fraction");
  ck_assert_msg(_is_equal(result, expected),
                "1 / 13 should be 0.0769230769230769230769230769");
}
END_TEST

START_TEST(test_div_simple_case) {
  s21_decimal num1 = {{0x7D, 0, 0, 0}};
  s21_decimal num2 = {{0x19, 0, 0, 0}};

  s21_decimal result;
  // 125/25 = 5
  s21_decimal expected = {{0x5, 0x0, 0x0, 0}};

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed on complex periodic fraction");
  ck_assert_msg(_is_equal(result, expected),
                "1 / 13 should be 0.0769230769230769230769230769");
}
END_TEST

#ifdef ENABLE_EXTENDED_TESTS
START_TEST(test_div_edge_case_1) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  // 4237574999123243252.047263256
  s21_decimal value_2 = {{0xc239b618, 0x01882ac2, 0x0db13d69, 0x90000}};
  s21_decimal result = {{0}};
  // 18696580598.728445112386526146
  s21_decimal expected = {{0xd0431bc2, 0x28da9b43, 0x3c6972f1, 0x120000}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_edge_case_2) {
  // 535873509258928592.89529592859
  s21_decimal value_1 = {{0x78e5ac1b, 0x946e3ff1, 0xad266af2, 0xb0000}};
  // -6534.0000000000000000000000003
  s21_decimal value_2 = {{0xbc000002, 0x8298bb10, 0xd31ffb31, 0x80190000}};
  s21_decimal result = {{0}};
  // -82013086816487.38795459074511
  s21_decimal expected = {{0x19de59cf, 0x7e4ec6f2, 0x1a7ff6aa, 0x800e0000}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_edge_case_3) {
  // 0.7922816251426433759354395033
  s21_decimal value_1 = {{0x99999999, 0x99999999, 0x19999999, 0x1c0000}};
  // 29228162514264337593543950335
  s21_decimal value_2 = {{0xafffffff, 0xc946f41a, 0x5e70f828, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_div_edge_case_4) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  // 0.000000000000003
  s21_decimal value_2 = {{0x3, 0x0, 0x0, 0xf0000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST
#endif

Suite *s21_div_suite() {
  Suite *ps = suite_create("div");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_div_by_null);
  tcase_add_test(tc, test_div_zero_by_number);
  tcase_add_test(tc, test_div_one_by_one);
  tcase_add_test(tc, test_div_ten_by_two);
  tcase_add_test(tc, test_div_with_different_scales);
  tcase_add_test(tc, test_div_with_negative);
  tcase_add_test(tc, test_div_large_numbers);
  tcase_add_test(tc, test_div_rounding);
  tcase_add_test(tc, test_div_periodic_fraction);
  tcase_add_test(tc, test_div_complex_periodic);
  tcase_add_test(tc, test_div_simple_case);

#ifdef ENABLE_EXTENDED_TESTS
  TCase *tc_extended = tcase_create("extended");

  tcase_add_test(tc_extended, test_div_edge_case_1);
  tcase_add_test(tc_extended, test_div_edge_case_2);
  tcase_add_test(tc_extended, test_div_edge_case_3);
  tcase_add_test(tc_extended, test_div_edge_case_4);

  suite_add_tcase(ps, tc_extended);
#endif

  suite_add_tcase(ps, tc);
  return ps;
}