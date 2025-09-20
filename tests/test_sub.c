#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

START_TEST(test_sub_negative_overflow) {
  s21_decimal a = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};  // -2^96 - 1
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  ck_assert_msg(s21_sub(a, b, &result) == S21_TOO_SMALL,
                "s21_sub doesn't return error on negative overflow");
}
END_TEST

/**/
START_TEST(test_sub_max_scale_num) {
  s21_decimal a = {{123456, 0, 0, 0x801C0000u}};
  s21_decimal b = {{5, 0, 0, 0x80190000u}};
  s21_decimal result;
  int code = s21_sub(a, b, &result);

  ck_assert_int_eq(code, 0);  // SUCCESS
  ck_assert_int_eq(result.bits[0], 118456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0x801C0000);
}
END_TEST

START_TEST(test_sub_equal_numbers) {
  s21_decimal a = {{42, 0, 0, 0}};
  s21_decimal b = {{42, 0, 0, 0}};
  s21_decimal result;

  int res = s21_sub(a, b, &result);
  ck_assert_msg(
      res == S21_SUCCESS,
      "s21_sub returned %d error code on subtraction of two equal numbers",
      res);

  for (int i = 0; i < 3; ++i) ck_assert_int_eq(result.bits[i], 0);
}
END_TEST

START_TEST(test_sub_zero) {
  s21_decimal number = {{0xDEADBEEF, 0xCAFEBABE, 0x01010101, 0}};
  s21_decimal result;
  s21_decimal zero = {{0, 0, 0, 0}};

  int res = s21_sub(number, zero, &result);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction zero", res);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));

  _set_sign(&zero, 1);
  res = s21_sub(number, zero, &result);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction zero", res);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));

  _set_scale(&zero, 11);
  res = s21_sub(number, zero, &result);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction zero", res);
  ck_assert_mem_eq(&result, &number, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_sub_negative_result) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_decimal result;

  int res = s21_sub(a, b, &result);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction 3 - 10", res);
  ck_assert_int_eq(result.bits[0], 7);
  ck_assert_int_eq(result.bits[3], 0x80000000);  // отрицательный
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

START_TEST(test_sub_same_sign) {
  s21_decimal minuend = {{444, 0, 0, 0x80000000}};
  s21_decimal subtrahend = {{44, 0, 0, 0}};
  s21_decimal difference;
  s21_decimal expected_difference = {{488, 0, 0, 0x80000000}};

  int res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction -444 - 44", res);
  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));

  _set_sign(&minuend, 0);
  _set_sign(&subtrahend, 1);
  _set_sign(&expected_difference, 0);

  res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction 444 - (-44)",
                res);
  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_sub_with_borrow) {
  s21_decimal minuend = {{0, 0, 0xFFFFFFFF, 0}};
  s21_decimal subtrahend = {{1, 0, 0, 0}};
  s21_decimal difference;
  s21_decimal expected_difference = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF - 1, 0}};

  int res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction with borrow",
                res);
  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));

  minuend = (s21_decimal){{0, 0, 0xF0000000, 0}};
  expected_difference =
      (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xF0000000 - 1, 0}};

  res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction with borrow",
                res);
  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_sub_from_zero) {
  s21_decimal minuend = {{0, 0, 0, 0}};
  s21_decimal subtrahend = {{0, 0xDEADBEEF, 0xCAFEBABE, 0}};
  s21_decimal difference;
  s21_decimal expected_difference = {{0, 0xDEADBEEF, 0xCAFEBABE, 0x80000000}};

  int res = s21_sub(minuend, subtrahend, &difference);
  ck_assert_msg(res == S21_SUCCESS,
                "s21_sub returned %d error code on subtraction with borrow",
                res);
  ck_assert_mem_eq(&difference, &expected_difference, sizeof(s21_decimal));
}
END_TEST

Suite *s21_sub_suite() {
  Suite *ps = suite_create("sub");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_negative_overflow);
  tcase_add_test(tc, test_sub_max_scale_num);
  tcase_add_test(tc, test_sub_equal_numbers);
  tcase_add_test(tc, test_sub_zero);
  tcase_add_test(tc, test_sub_negative_result);
  tcase_add_test(tc, test_sub_small_decimal);
  tcase_add_test(tc, test_sub_same_sign);
  tcase_add_test(tc, test_sub_with_borrow);
  tcase_add_test(tc, test_sub_from_zero);

  suite_add_tcase(ps, tc);
  return ps;
}