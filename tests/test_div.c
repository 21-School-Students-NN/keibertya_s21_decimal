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
  ck_assert_msg(_is_equal(result, expected),
                "0 / number should be 0");
}
END_TEST

START_TEST(test_div_one_by_one) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{1, 0, 0, 0}};

  ck_assert_msg(s21_div(one, one, &result) == S21_SUCCESS,
                "s21_div failed on 1 / 1");
  ck_assert_msg(_is_equal(result, expected),
                "1 / 1 should be 1");
}
END_TEST

START_TEST(test_div_ten_by_two) {
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{5, 0, 0, 0}};

  ck_assert_msg(s21_div(ten, two, &result) == S21_SUCCESS,
                "s21_div failed on 10 / 2");
  ck_assert_msg(_is_equal(result, expected),
                "10 / 2 should be 5");
}
END_TEST

START_TEST(test_div_with_different_scales) {
  s21_decimal num1 = {{100, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  _set_scale(&num1, 1); // 10.0
  _set_scale(&num2, 0); // 10

  s21_decimal result;
  s21_decimal expected = {{10, 0, 0, 0}};
  _set_scale(&expected, 1); // 1.0
  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed on numbers with different scales");
  ck_assert_msg(_is_equal(result, expected),
                "10.0 / 10 should be 1.0");

}
END_TEST

START_TEST(test_div_with_negative) {
  s21_decimal num1 = {{100, 0, 0, 0}};
  s21_decimal num2 = {{10, 0, 0, 0}};
  _set_sign(&num1, 1); // -100

  s21_decimal result;
  s21_decimal expected = {{10, 0, 0, 0}};
  _set_sign(&expected, 1); // -10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with negative dividend");
  ck_assert_msg(_is_equal(result, expected),
                "-100 / 10 should be -10");

  _set_sign(&num1, 0); // 100
  _set_sign(&num2, 1); // -10
  _set_sign(&expected, 1); // -10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with negative divisor");
  ck_assert_msg(_is_equal(result, expected),
                "100 / -10 should be -10");

  _set_sign(&num1, 1); // -100
  _set_sign(&expected, 0); // 10

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with both negative");
  ck_assert_msg(_is_equal(result, expected),
                "-100 / -10 should be 10");
}
END_TEST

START_TEST(test_div_large_numbers) {
  s21_decimal num1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}; // max 96-bit
  s21_decimal num2 = {{1, 0, 0, 0}};

  s21_decimal result;
  s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with large numbers");
  ck_assert_msg(_is_equal(result, expected),
                "Max value / 1 should be max value");
}
END_TEST

// START_TEST(test_div_fractional_result) {
//   s21_decimal num1 = {{1, 0, 0, 0}};
//   s21_decimal num2 = {{3, 0, 0, 0}};

//   s21_decimal result;
  
//   // 1/3 should give 0.333... with banking rounding
//   ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
//                 "s21_div failed with fractional result");
  
//   // Check that scale is appropriate
//   ck_assert_msg(_get_scale(&result) > 0,
//                 "Fractional division should result in scaled decimal");
// }
// END_TEST

// START_TEST(test_div_precision_limits) {
//   s21_decimal num1 = {{1, 0, 0, 0}};
//   s21_decimal num2 = {{7, 0, 0, 0}};
//   _set_scale(&num1, 28); // Very small number

//   s21_decimal result;
  
//   ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
//                 "s21_div failed at precision limits");
  
//   // Should handle maximum scale correctly
//   ck_assert_msg(_get_scale(&result) <= 28,
//                 "Result scale should not exceed maximum");
// }
// END_TEST

START_TEST(test_div_rounding) {
  s21_decimal num1 = {{10, 0, 0, 0}};
  s21_decimal num2 = {{4, 0, 0, 0}};

  s21_decimal result;
  s21_decimal expected = {{25, 0, 0, 0}};
  _set_scale(&expected, 1); // 2.5
  ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
                "s21_div failed with rounding test");
  ck_assert_msg(_is_equal(result, expected),
                "10 / 4 should be 2.5");
}
END_TEST

// START_TEST(test_div_banking_rounding) {
//   s21_decimal num1 = {{1, 0, 0, 0}};
//   s21_decimal num2 = {{2, 0, 0, 0}};

//   s21_decimal result;
  
//   // Test banking rounding: 0.5 should round to 0 (even)
//   ck_assert_msg(s21_div(num1, num2, &result) == S21_SUCCESS,
//                 "s21_div failed banking rounding test");
//   // The result should be 0.5 which gets rounded based on banking rules
//   ck_assert_msg(_get_scale(&result) > 0,
//                 "Banking rounding should be applied");
// }
// END_TEST

// START_TEST(test_div_overflow_handling) {
//   s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
//   s21_decimal min_val = {{1, 0, 0, 0}};
//   _set_scale(&min_val, 28); // Very small number

//   s21_decimal result;
  
//   // This should trigger overflow handling
//   int ret = s21_div(max_val, min_val, &result);
  
//   // Should either succeed with proper scaling or return error
//   ck_assert_msg(ret == 0 || ret == S21_TOO_LARGE,
//                 "s21_div should handle overflow gracefully");
// }
// END_TEST

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
  // tcase_add_test(tc, test_div_fractional_result);
  // tcase_add_test(tc, test_div_precision_limits);
  tcase_add_test(tc, test_div_rounding);
  // tcase_add_test(tc, test_div_banking_rounding);
  // tcase_add_test(tc, test_div_overflow_handling);
  suite_add_tcase(ps, tc);
  return ps;
}