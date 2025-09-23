#include <check.h>

#include "../include/s21_decimal.h"
#include "../include/s21_suites.h"

START_TEST(test_add_1) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  // 0.49
  s21_decimal value_2 = {{0x31, 0x0, 0x0, 0x20000}};
  s21_decimal result = {{0}};
  // 79228162514264337593543950335
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_2) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  // -0.49
  s21_decimal value_2 = {{0x31, 0x0, 0x0, 0x80020000}};
  s21_decimal result = {{0}};
  // 79228162514264337593543950335
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_3) {
  // -79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  // -0.49
  s21_decimal value_2 = {{0x31, 0x0, 0x0, 0x80020000}};
  s21_decimal result = {{0}};
  // -79228162514264337593543950335
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_4) {
  // 39228162514264337593543950335
  s21_decimal value_1 = {{0xbfffffff, 0x076bf67b, 0x7ec0c687, 0x0}};
  // 31228162514264337593543950335
  s21_decimal value_2 = {{0x7fffffff, 0x08e7f494, 0x64e754a2, 0x0}};
  s21_decimal result = {{0}};
  // 70456325028528675187087900670
  s21_decimal expected = {{0x3ffffffe, 0x1053eb10, 0xe3a81b29, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_5) {
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

START_TEST(test_add_6) {
  // 4237574999123243252.047263256
  s21_decimal value_1 = {{0xc239b618, 0x01882ac2, 0x0db13d69, 0x90000}};
  // 4237574999123243252.047263256
  s21_decimal value_2 = {{0xc239b618, 0x01882ac2, 0x0db13d69, 0x80090000}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_7) {
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

START_TEST(test_add_8) {
  // 0.1000000000000000000000000125
  s21_decimal value_1 = {{0xe800007d, 0x9fd0803c, 0x033b2e3c, 0x1c0000}};
  // -0.1000000000000000000000000124
  s21_decimal value_2 = {{0xe800007c, 0x9fd0803c, 0x033b2e3c, 0x801c0000}};
  s21_decimal result = {{0}};
  // -0.0000000000000000000000000001
  s21_decimal expected = {{0x1, 0x0, 0x0, 0x1c0000}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_9) {
  // 0.1000000000000000000000000125
  s21_decimal value_1 = {{0xe800007d, 0x9fd0803c, 0x033b2e3c, 0x1c0000}};
  // 0
  s21_decimal value_2 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  // 0.1000000000000000000000000125
  s21_decimal expected = {{0xe800007d, 0x9fd0803c, 0x033b2e3c, 0x1c0000}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_10) {
  // 0
  s21_decimal value_1 = {{0x0, 0x0, 0x0, 0x0}};
  // 0.1000000000000000000000000125
  s21_decimal value_2 = {{0xe800007d, 0x9fd0803c, 0x033b2e3c, 0x1c0000}};
  s21_decimal result = {{0}};
  // 0.1000000000000000000000000125
  s21_decimal expected = {{0xe800007d, 0x9fd0803c, 0x033b2e3c, 0x1c0000}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_11) {
  // 0
  s21_decimal value_1 = {{0x0, 0x0, 0x0, 0x0}};
  // 0
  s21_decimal value_2 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_uncorrect_1) {
  // 0.7922816251426433759354395033
  s21_decimal value_1 = {{0x99999999, 0x99999999, 0x19999999, 0x1c0000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_uncorrect_2) {
  // 0.5
  s21_decimal value_1 = {{0x5, 0x0, 0x0, 0x10000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_uncorrect_3) {
  // -39614081257132168796771975168
  s21_decimal value_1 = {{0x0, 0x0, 0x80000000, 0x80000000}};
  // -39614081257132168796771975168
  s21_decimal value_2 = {{0x0, 0x0, 0x80000000, 0x80000000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_SMALL);
}
END_TEST

START_TEST(test_sub_1) {
  // 39614081257132168796771975167
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x0}};
  // 0.5
  s21_decimal value_2 = {{0x5, 0x0, 0x0, 0x10000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  // 39614081257132168796771975166
  s21_decimal expected = {{0xfffffffe, 0xffffffff, 0x7fffffff, 0x0}};
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_2) {
  // 39614081257132168796771975167
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x0}};
  // 0.49
  s21_decimal value_2 = {{0x31, 0x0, 0x0, 0x20000}};
  s21_decimal result = {{0}};
  // 39614081257132168796771975167
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_3) {
  // -39614081257132168796771975167
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x80000000}};
  // -39614081257132168796771975167
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x80000000}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_4) {
  // -79228162514264337593543950334
  s21_decimal value_1 = {{0xfffffffe, 0xffffffff, 0xffffffff, 0x80000000}};
  // 1.49
  s21_decimal value_2 = {{0x00000095, 0x00000000, 0x00000000, 0x20000}};
  s21_decimal result = {{0}};
  // -79228162514264337593543950335
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_5) {
  // -1922856.251426433759354395034
  s21_decimal value_1 = {{0xf299999a, 0x4239bf16, 0x06368ca6, 0x80150000}};
  // 2467868965996901.0467496723455
  s21_decimal value_2 = {{0x13d303ff, 0x2bf462f5, 0x4fbdbbcf, 0xd0000}};
  s21_decimal result = {{0}};
  // -2467868967919757.2981761061049
  s21_decimal expected = {{0xcff130b9, 0x36cdf6af, 0x4fbdbbd0, 0x800d0000}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_uncorrect_1) {
  // 39614081257132168796771975168
  s21_decimal value_1 = {{0x0, 0x0, 0x80000000, 0x0}};
  // -39614081257132168796771975168
  s21_decimal value_2 = {{0x0, 0x0, 0x80000000, 0x80000000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_sub_uncorrect_2) {
  // -39614081257132168796771975168
  s21_decimal value_1 = {{0x0, 0x0, 0x80000000, 0x80000000}};
  // 39614081257132168796771975168
  s21_decimal value_2 = {{0x0, 0x0, 0x80000000, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_SMALL);
}
END_TEST

START_TEST(test_sub_uncorrect_3) {
  // -79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  // 0.5000000000000000000000000001
  s21_decimal value_2 = {{0x88000001, 0x1f128130, 0x1027e72f, 0x1c0000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_SMALL);
}
END_TEST

START_TEST(test_sub_uncorrect_4) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  // -0.5000000000000000000000000001
  s21_decimal value_2 = {{0x88000001, 0x1f128130, 0x1027e72f, 0x801c0000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_mul_1) {
  // 0.1
  s21_decimal value_1 = {{0x1, 0x0, 0x0, 0x10000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  // 7.9228162514264337593543950335
  s21_decimal expected = {{0xffffffff, 0xffffffff, 0xffffffff, 0x10000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_2) {
  // -19228562514.264337593543950345
  s21_decimal value_1 = {{0x7a000009, 0x964176e5, 0x3e217e7e, 0x80120000}};
  // 2467868965996901046.7496723455
  s21_decimal value_2 = {{0x13d303ff, 0x2bf462f5, 0x4fbdbbcf, 0xa0000}};
  s21_decimal result = {{0}};
  // -47453572689684302651351529048
  s21_decimal expected = {{0x334e7658, 0x1e234cf2, 0x9954ace2, 0x80000000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_3) {
  // -2328562514.264337593543950345
  s21_decimal value_1 = {{0xf6000009, 0xd630ffac, 0x0786244a, 0x80120000}};
  // -589665996901046.7496723455
  s21_decimal value_2 = {{0x163303ff, 0x93d3238a, 0x0004e0aa, 0x800a0000}};
  s21_decimal result = {{0}};
  // 1373074136320088519264082.6932
  s21_decimal expected = {{0x55ea0e34, 0xca7b9f35, 0x2c5dcd94, 0x40000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_4) {
  // 63637540.749672345535439505672
  s21_decimal value_1 = {{0xd4450908, 0x9f1939fc, 0xcd9fbd6c, 0x150000}};
  // -474762955966586.40659969010467
  s21_decimal value_2 = {{0xff44eb23, 0x5f81247b, 0x996778a5, 0x800e0000}};
  s21_decimal result = {{0}};
  // 1373074136320088519264082.6932
  s21_decimal expected = {{0x4fca447e, 0x5d16456e, 0x619f6606, 0x80060000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_5) {
  // 0.7922816251426433759354395033
  s21_decimal value_1 = {{0x99999999, 0x99999999, 0x19999999, 0x1c0000}};
  // 0.0000000000000000000000000001
  s21_decimal value_2 = {{0x1, 0x0, 0x0, 0x1c0000}};
  s21_decimal result = {{0}};
  // 0.0000000000000000000000000001
  s21_decimal expected = {{0x1, 0x0, 0x0, 0x1c0000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_6) {
  // 12125522270318998713428826.191
  s21_decimal value_1 = {{0xc073584f, 0x666a6978, 0x272dff37, 0x30000}};
  // -6534.0000000000000000000000002
  s21_decimal value_2 = {{0xbc000002, 0x8298bb10, 0xd31ffb31, 0x80190000}};
  s21_decimal result = {{0}};
  // 79228162514264337593543950334
  s21_decimal expected = {{0xfffffffe, 0xffffffff, 0xffffffff, 0x80000000}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_7) {
  // 535873509258928592.89529592859
  s21_decimal value_1 = {{0x78e5ac1b, 0x946e3ff1, 0xad266af2, 0xb0000}};
  // 0
  s21_decimal value_2 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_8) {
  // 0
  s21_decimal value_1 = {{0x0, 0x0, 0x0, 0x0}};
  // -535873509258928592.89529592859
  s21_decimal value_2 = {{0x78e5ac1b, 0x946e3ff1, 0xad266af2, 0x800b0000}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_uncorrect_1) {
  // 0.4922816251426433759354395033
  s21_decimal value_1 = {{0xe1999999, 0xba2818e2, 0x0fe80ee3, 0x1c0000}};
  // 0.0000000000000000000000000001
  s21_decimal value_2 = {{0x1, 0x0, 0x0, 0x1c0000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_mul_uncorrect_2) {
  // 4922816251426433759354395033
  s21_decimal value_1 = {{0xe1999999, 0xba2818e2, 0x0fe80ee3, 0x0}};
  // 25
  s21_decimal value_2 = {{0x19, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_mul_uncorrect_3) {
  // 12125522270318998713428826.191
  s21_decimal value_1 = {{0xc073584f, 0x666a6978, 0x272dff37, 0x30000}};
  // -6534.0000000000000000000000003
  s21_decimal value_2 = {{0xbc000003, 0x8298bb10, 0xd31ffb31, 0x80190000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_SMALL);
}
END_TEST

START_TEST(test_mul_uncorrect_4) {
  // 12125522270318998713428826.191
  s21_decimal value_1 = {{0xc073584f, 0x666a6978, 0x272dff37, 0x30000}};
  // 6534.0000000000000000000000003
  s21_decimal value_2 = {{0xbc000003, 0x8298bb10, 0xd31ffb31, 0x190000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_div_1) {
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

START_TEST(test_div_2) {
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

START_TEST(test_div_3) {
  // -10
  s21_decimal value_1 = {{0xa, 0x0, 0x0, 0x80000000}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  // 0.0000000000000000000000000001
  s21_decimal expected = {{0x1, 0x0, 0x0, 0x801c0000}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_4) {
  // 0
  s21_decimal value_1 = {{0x0, 0x0, 0x0, 0x0}};
  // 79228162514264337593543950335
  s21_decimal value_2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0}};
  s21_decimal result = {{0}};
  // 0
  s21_decimal expected = {{0x0, 0x0, 0x0, 0x0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_5) {
  // 1
  s21_decimal value_1 = {{0x1, 0x0, 0x0, 0x0}};
  // 2
  s21_decimal value_2 = {{0x2, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  // 0.5
  s21_decimal expected = {{0x5, 0, 0, 0x010000}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_SUCCESS);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_uncorrect_1) {
  // 0.7922816251426433759354395033
  s21_decimal value_1 = {{0x99999999, 0x99999999, 0x19999999, 0x1c0000}};
  // 29228162514264337593543950335
  s21_decimal value_2 = {{0xafffffff, 0xc946f41a, 0x5e70f828, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_div_uncorrect_2) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  // 0.000000000000003
  s21_decimal value_2 = {{0x3, 0x0, 0x0, 0xf0000}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_div_uncorrect_3) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  // 0
  s21_decimal value_2 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal result = {{0}};
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_code_return, S21_DIV_BY_ZERO);
}
END_TEST

START_TEST(test_add_null) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal *result = NULL;
  int s21_code_return = s21_add(value_1, value_2, result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_add_uncorrect_free_bits1) {
  s21_decimal value_1 = {{0, 0, 0, 0x80000001}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_add_uncorrect_free_bits2) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal value_2 = {{0, 0, 0, 0x01000000}};
  s21_decimal result;
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_add_uncorrect_scale1) {
  s21_decimal value_1 = {{5, 0, 0, 0x1E0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_add_uncorrect_scale2) {
  s21_decimal value_1 = {{1, 0, 0, 0xFF0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_add(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_sub_null) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal *result = NULL;
  int s21_code_return = s21_sub(value_1, value_2, result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_sub_uncorrect_free_bits1) {
  s21_decimal value_1 = {{0, 0, 0, 0x80000001}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_sub_uncorrect_free_bits2) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal value_2 = {{0, 0, 0, 0x01000000}};
  s21_decimal result;
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_sub_uncorrect_scale1) {
  s21_decimal value_1 = {{5, 0, 0, 0x1E0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_sub_uncorrect_scale2) {
  s21_decimal value_1 = {{1, 0, 0, 0xFF0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_sub(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_mul_null) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal *result = NULL;
  int s21_code_return = s21_mul(value_1, value_2, result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_mul_uncorrect_free_bits1) {
  s21_decimal value_1 = {{0, 0, 0, 0x80000001}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_mul_uncorrect_free_bits2) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal value_2 = {{0, 0, 0, 0x01000000}};
  s21_decimal result;
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_mul_uncorrect_scale1) {
  s21_decimal value_1 = {{5, 0, 0, 0x1E0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_mul_uncorrect_scale2) {
  s21_decimal value_1 = {{1, 0, 0, 0xFF0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_mul(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_div_null) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal *result = NULL;
  int s21_code_return = s21_div(value_1, value_2, result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_div_uncorrect_free_bits1) {
  s21_decimal value_1 = {{0, 0, 0, 0x80000001}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_div_uncorrect_free_bits2) {
  // 26790900105265827596743072504
  s21_decimal value_1 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal value_2 = {{0, 0, 0, 0x01000000}};
  s21_decimal result;
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_div_uncorrect_scale1) {
  s21_decimal value_1 = {{5, 0, 0, 0x1E0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

START_TEST(test_div_uncorrect_scale2) {
  s21_decimal value_1 = {{1, 0, 0, 0xFF0000}};
  // 26790900105265827596743072504
  s21_decimal value_2 = {{0x2be1daf8, 0xd93baf9b, 0x5690e9c4, 0x0}};
  s21_decimal result;
  int s21_code_return = s21_div(value_1, value_2, &result);
  ck_assert_int_ne(s21_code_return, S21_SUCCESS);
}
END_TEST

Suite *arithmetic_suite(void) {
  Suite *s = suite_create("s21_arithmetic");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_add_1);
  tcase_add_test(tc, test_add_2);
  tcase_add_test(tc, test_add_3);
  tcase_add_test(tc, test_add_4);
  tcase_add_test(tc, test_add_5);
  tcase_add_test(tc, test_add_6);
  tcase_add_test(tc, test_add_7);
  tcase_add_test(tc, test_add_8);
  tcase_add_test(tc, test_add_9);
  tcase_add_test(tc, test_add_10);
  tcase_add_test(tc, test_add_11);
  tcase_add_test(tc, test_add_uncorrect_1);
  tcase_add_test(tc, test_add_uncorrect_2);
  tcase_add_test(tc, test_add_uncorrect_3);

  tcase_add_test(tc, test_sub_1);
  tcase_add_test(tc, test_sub_2);
  tcase_add_test(tc, test_sub_3);
  tcase_add_test(tc, test_sub_4);
  tcase_add_test(tc, test_sub_5);
  tcase_add_test(tc, test_sub_uncorrect_1);
  tcase_add_test(tc, test_sub_uncorrect_2);
  tcase_add_test(tc, test_sub_uncorrect_3);
  tcase_add_test(tc, test_sub_uncorrect_4);

  tcase_add_test(tc, test_mul_1);
  tcase_add_test(tc, test_mul_2);
  tcase_add_test(tc, test_mul_3);
  tcase_add_test(tc, test_mul_4);
  tcase_add_test(tc, test_mul_5);
  tcase_add_test(tc, test_mul_6);
  tcase_add_test(tc, test_mul_7);
  tcase_add_test(tc, test_mul_8);
  tcase_add_test(tc, test_mul_uncorrect_1);
  tcase_add_test(tc, test_mul_uncorrect_2);
  tcase_add_test(tc, test_mul_uncorrect_3);
  tcase_add_test(tc, test_mul_uncorrect_4);

  tcase_add_test(tc, test_div_1);
  tcase_add_test(tc, test_div_2);
  tcase_add_test(tc, test_div_3);
  tcase_add_test(tc, test_div_4);
  tcase_add_test(tc, test_div_5);
  tcase_add_test(tc, test_div_uncorrect_1);
  tcase_add_test(tc, test_div_uncorrect_2);
  tcase_add_test(tc, test_div_uncorrect_3);

  tcase_add_test(tc, test_add_null);
  tcase_add_test(tc, test_add_uncorrect_free_bits1);
  tcase_add_test(tc, test_add_uncorrect_free_bits2);
  tcase_add_test(tc, test_add_uncorrect_scale1);
  tcase_add_test(tc, test_add_uncorrect_scale2);

  tcase_add_test(tc, test_sub_null);
  tcase_add_test(tc, test_sub_uncorrect_free_bits1);
  tcase_add_test(tc, test_sub_uncorrect_free_bits2);
  tcase_add_test(tc, test_sub_uncorrect_scale1);
  tcase_add_test(tc, test_sub_uncorrect_scale2);

  tcase_add_test(tc, test_mul_null);
  tcase_add_test(tc, test_mul_uncorrect_free_bits1);
  tcase_add_test(tc, test_mul_uncorrect_free_bits2);
  tcase_add_test(tc, test_mul_uncorrect_scale1);
  tcase_add_test(tc, test_mul_uncorrect_scale2);

  tcase_add_test(tc, test_div_null);
  tcase_add_test(tc, test_div_uncorrect_free_bits1);
  tcase_add_test(tc, test_div_uncorrect_free_bits2);
  tcase_add_test(tc, test_div_uncorrect_scale1);
  tcase_add_test(tc, test_div_uncorrect_scale2);

  suite_add_tcase(s, tc);
  return s;
}