#include "../include/s21_decimal.h"
#include "../include/s21_suites.h"

START_TEST(test_add_positive_no_overflow) {
    s21_decimal a = {{123, 0, 0, 0}};
    s21_decimal b = {{456, 0, 0, 0}};
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, 0);
    ck_assert_uint_eq(result.parts.low, 579);
    ck_assert_uint_eq(result.parts.sign, 0);
}
END_TEST

START_TEST(test_add_overflow_into_mid) {
    s21_decimal a = {{0xFFFFFFFF, 0, 0, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, 0);
    ck_assert_uint_eq(result.parts.low, 0);
    ck_assert_uint_eq(result.parts.mid, 1);  // Carry propagated
}
END_TEST

START_TEST(test_add_negative_result) {
    s21_decimal a = {{100, 0, 0, 1}};  // -100
    s21_decimal b = {{50, 0, 0, 1}};    // -50
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, 0);
    ck_assert_uint_eq(result.parts.low, 150);
    ck_assert_uint_eq(result.parts.sign, 1);  // Negative
}
END_TEST

START_TEST(test_add_max_overflow) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // 2^96 - 1
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, 1);  // Expected "too large" error
}
END_TEST

START_TEST(test_add_different_scales) {
    s21_decimal a = {{1234, 0, 0, 2}};  // 12.34
    s21_decimal b = {{567, 0, 0, 1}};    // 56.7
    s21_decimal result;
    int status = s21_add(a, b, &result);

    ck_assert_int_eq(status, 0);
    ck_assert_uint_eq(result.parts.low, 1234 + 5670);  // Scaled to 10^-2
    ck_assert_uint_eq(result.parts.scale, 2);
}
END_TEST

Suite* s21_add_suite(){
    Suite* ps;
    
    return ps;
}