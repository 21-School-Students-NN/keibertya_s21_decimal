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
}
END_TEST

START_TEST(test_s21_div_infinite_decimal) {
  s21_decimal num1 = {{123, 0, 0, 0}};
  s21_decimal num2 = {{9, 0, 0, 0}};

  s21_decimal result;
  s21_decimal expected = {{0x62AAAAAB, 0x88218340, 0x2C28CD3C,
                           0}};  // 13.666666666666666666666666667
  _set_scale(&expected, 27);
  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with rounding test");
  ck_assert_msg(_is_equal(result, expected),
                "123 / 9 should be 13.666666666666666666666666667");
}

START_TEST(test_div_periodic_fraction) {
  s21_decimal num1 = {{1, 0, 0, 0}};
  s21_decimal num2 = {{3, 0, 0, 0}};

  s21_decimal result;

  // 1/3 = 0.3333333333333333333333333333 -> to hex: AC544CA14B700CB05555555
  s21_decimal expected = {{0x05555555, 0x14B700CB, 0x0AC544CA, 0}};
  _set_scale(&expected, 28);

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
  tcase_add_test(tc, test_s21_div_infinite_decimal);
  suite_add_tcase(ps, tc);
  return ps;
}