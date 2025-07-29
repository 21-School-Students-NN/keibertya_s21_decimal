#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

// Test helper function to initialize decimal to zero
void init_decimal_zero(s21_decimal *dec) {
    if (dec) memset(dec, 0, sizeof(s21_decimal));
}

// Test helper function to print decimal bits for debugging
void print_decimal_bits(const s21_decimal *dec) {
    if (!dec) return;
    printf("Decimal bits: [%u, %u, %u, %u]\n", 
           dec->bits[0], dec->bits[1], dec->bits[2], dec->bits[3]);
}

// Test helper function to check if decimal is zero
int is_decimal_zero(const s21_decimal *dec) {
    if (!dec) return 0;
    return (dec->bits[0] == 0 && dec->bits[1] == 0 && 
            dec->bits[2] == 0 && (dec->bits[3] & 0x7FFFFFFF) == 0);
}

// =============================================================================
// s21_get_scale TESTS
// =============================================================================

START_TEST(test_get_scale_zero_decimal) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    unsigned char scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 0);
}
END_TEST

START_TEST(test_get_scale_normal_values) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test scale 5
    dec.bits[3] = 5 << 16;
    unsigned char scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 5);
    
    // Test scale 10
    dec.bits[3] = 10 << 16;
    scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 10);
    
    // Test scale 28 (max)
    dec.bits[3] = 28 << 16;
    scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 28);
}
END_TEST

START_TEST(test_get_scale_with_sign) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Negative decimal with scale 15
    dec.bits[3] = (1 << 31) | (15 << 16);
    unsigned char scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 15);
    
    // Positive decimal with scale 20
    dec.bits[3] = (0 << 31) | (20 << 16);
    scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 20);
}
END_TEST

START_TEST(test_get_scale_null_pointer) {
    unsigned char scale = s21_get_scale(NULL);
    ck_assert_uint_eq(scale, S21_ERROR);
}
END_TEST

START_TEST(test_get_scale_edge_cases) {
    s21_decimal dec;
    
    // Test with all bits set in bits[3] except scale
    dec.bits[0] = 0xFFFFFFFF;
    dec.bits[1] = 0xFFFFFFFF;
    dec.bits[2] = 0xFFFFFFFF;
    dec.bits[3] = 0x7F00FFFF; // All bits set except scale and sign
    
    unsigned char scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 0);
    
    // Test with specific scale value
    dec.bits[3] = 0x7F00FFFF | (25 << 16);
    scale = s21_get_scale(&dec);
    ck_assert_uint_eq(scale, 25);
}
END_TEST

// =============================================================================
// s21_set_scale TESTS
// =============================================================================

START_TEST(test_set_scale_valid_values) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test setting scale to 0
    int result = s21_set_scale(&dec, 0);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 0);
    
    // Test setting scale to 10
    result = s21_set_scale(&dec, 10);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 10);
    
    // Test setting scale to 28 (max)
    result = s21_set_scale(&dec, 28);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 28);
}
END_TEST

START_TEST(test_set_scale_invalid_values) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test invalid scale 29
    int result = s21_set_scale(&dec, 29);
    ck_assert_int_eq(result, S21_ERROR);
    
    // Test invalid scale 255
    result = s21_set_scale(&dec, 255);
    ck_assert_int_eq(result, S21_ERROR);
    
    // Test invalid scale 100
    result = s21_set_scale(&dec, 100);
    ck_assert_int_eq(result, S21_ERROR);
}
END_TEST

START_TEST(test_set_scale_null_pointer) {
    int result = s21_set_scale(NULL, 5);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_set_scale_preserve_sign) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Set sign bit first
    dec.bits[3] = 1 << 31;
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
    
    // Set scale while preserving sign
    int result = s21_set_scale(&dec, 7);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 7);
    ck_assert_uint_eq(s21_get_sign(&dec), 1); // Sign should be preserved
}
END_TEST

START_TEST(test_set_scale_all_valid_values) {
    s21_decimal dec;
    
    // Test all valid scale values (0-28)
    for (int i = 0; i <= 28; i++) {
        init_decimal_zero(&dec);
        int result = s21_set_scale(&dec, i);
        ck_assert_int_eq(result, 0);
        ck_assert_uint_eq(s21_get_scale(&dec), i);
    }
}
END_TEST

// =============================================================================
// s21_get_sign TESTS
// =============================================================================

START_TEST(test_get_sign_zero_decimal) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    unsigned char sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 0);
}
END_TEST

START_TEST(test_get_sign_positive_decimal) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Explicitly set to positive
    dec.bits[3] = 0;
    unsigned char sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 0);
}
END_TEST

START_TEST(test_get_sign_negative_decimal) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Set sign bit
    dec.bits[3] = 1 << 31;
    unsigned char sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 1);
}
END_TEST

START_TEST(test_get_sign_with_scale) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Negative decimal with scale 10
    dec.bits[3] = (1 << 31) | (10 << 16);
    unsigned char sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 1);
    
    // Positive decimal with scale 15
    dec.bits[3] = (0 << 31) | (15 << 16);
    sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 0);
}
END_TEST

START_TEST(test_get_sign_null_pointer) {
    unsigned char sign = s21_get_sign(NULL);
    ck_assert_uint_eq(sign, S21_ERROR);
}
END_TEST

START_TEST(test_get_sign_edge_cases) {
    s21_decimal dec;
    
    // Test with all bits set in bits[3] except sign
    dec.bits[0] = 0xFFFFFFFF;
    dec.bits[1] = 0xFFFFFFFF;
    dec.bits[2] = 0xFFFFFFFF;
    dec.bits[3] = 0x7FFFFFFF; // All bits set except sign
    
    unsigned char sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 0);
    
    // Test with sign bit set
    dec.bits[3] = 0xFFFFFFFF; // All bits set including sign
    sign = s21_get_sign(&dec);
    ck_assert_uint_eq(sign, 1);
}
END_TEST

// =============================================================================
// s21_set_sign TESTS
// =============================================================================

START_TEST(test_set_sign_valid_values) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test setting sign to positive (0)
    int result = s21_set_sign(&dec, 0);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 0);
    
    // Test setting sign to negative (1)
    result = s21_set_sign(&dec, 1);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
}
END_TEST

START_TEST(test_set_sign_invalid_values) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test invalid sign 2
    int result = s21_set_sign(&dec, 2);
    ck_assert_int_eq(result, 1);
    
    // Test invalid sign 255
    result = s21_set_sign(&dec, 255);
    ck_assert_int_eq(result, 1);
    
    // Test invalid sign 100
    result = s21_set_sign(&dec, 100);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_set_sign_null_pointer) {
    int result = s21_set_sign(NULL, 0);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_set_sign_preserve_scale) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Set scale first
    dec.bits[3] = 15 << 16;
    ck_assert_uint_eq(s21_get_scale(&dec), 15);
    
    // Set sign while preserving scale
    int result = s21_set_sign(&dec, 1);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
    ck_assert_uint_eq(s21_get_scale(&dec), 15); // Scale should be preserved
}
END_TEST

START_TEST(test_set_sign_multiple_changes) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test multiple sign changes
    int result = s21_set_sign(&dec, 0);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 0);
    
    result = s21_set_sign(&dec, 1);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
    
    result = s21_set_sign(&dec, 0);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 0);
}
END_TEST

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

START_TEST(test_integration_scale_and_sign) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Set both scale and sign
    int result = s21_set_scale(&dec, 25);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 25);
    
    result = s21_set_sign(&dec, 1);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
    ck_assert_uint_eq(s21_get_scale(&dec), 25); // Scale should still be 25
    
    // Change scale while preserving sign
    result = s21_set_scale(&dec, 10);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 10);
    ck_assert_uint_eq(s21_get_sign(&dec), 1); // Sign should be preserved
}
END_TEST

START_TEST(test_integration_memory_consistency) {
    s21_decimal dec1, dec2;
    init_decimal_zero(&dec1);
    init_decimal_zero(&dec2);
    
    // Set different values
    s21_set_scale(&dec1, 15);
    s21_set_sign(&dec1, 1);
    
    s21_set_scale(&dec2, 20);
    s21_set_sign(&dec2, 0);
    
    // Verify they are different
    ck_assert_uint_eq(s21_get_scale(&dec1), 15);
    ck_assert_uint_eq(s21_get_sign(&dec1), 1);
    ck_assert_uint_eq(s21_get_scale(&dec2), 20);
    ck_assert_uint_eq(s21_get_sign(&dec2), 0);
    
    // Verify memory is different
    ck_assert_int_ne(memcmp(&dec1, &dec2, sizeof(s21_decimal)), 0);
}
END_TEST

START_TEST(test_integration_zero_handling) {
    s21_decimal dec;
    init_decimal_zero(&dec);
    
    // Test that zero decimal has correct default values
    ck_assert_uint_eq(s21_get_scale(&dec), 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 0);
    
    // Test setting scale and sign on zero
    int result = s21_set_scale(&dec, 5);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_scale(&dec), 5);
    
    result = s21_set_sign(&dec, 1);
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(s21_get_sign(&dec), 1);
}
END_TEST

// =============================================================================
// SUITE CREATION
// =============================================================================

Suite *s21_helpers_suite(void) {
    Suite *s = suite_create("helpers");
    TCase *tc = tcase_create("core");
    
    // Add s21_get_scale tests
    tcase_add_test(tc, test_get_scale_zero_decimal);
    tcase_add_test(tc, test_get_scale_normal_values);
    tcase_add_test(tc, test_get_scale_with_sign);
    tcase_add_test(tc, test_get_scale_null_pointer);
    tcase_add_test(tc, test_get_scale_edge_cases);
    
    // Add s21_set_scale tests
    tcase_add_test(tc, test_set_scale_valid_values);
    tcase_add_test(tc, test_set_scale_invalid_values);
    tcase_add_test(tc, test_set_scale_null_pointer);
    tcase_add_test(tc, test_set_scale_preserve_sign);
    tcase_add_test(tc, test_set_scale_all_valid_values);
    
    // Add s21_get_sign tests
    tcase_add_test(tc, test_get_sign_zero_decimal);
    tcase_add_test(tc, test_get_sign_positive_decimal);
    tcase_add_test(tc, test_get_sign_negative_decimal);
    tcase_add_test(tc, test_get_sign_with_scale);
    tcase_add_test(tc, test_get_sign_null_pointer);
    tcase_add_test(tc, test_get_sign_edge_cases);
    
    // Add s21_set_sign tests
    tcase_add_test(tc, test_set_sign_valid_values);
    tcase_add_test(tc, test_set_sign_invalid_values);
    tcase_add_test(tc, test_set_sign_null_pointer);
    tcase_add_test(tc, test_set_sign_preserve_scale);
    tcase_add_test(tc, test_set_sign_multiple_changes);
    
    // Add integration tests
    tcase_add_test(tc, test_integration_scale_and_sign);
    tcase_add_test(tc, test_integration_memory_consistency);
    tcase_add_test(tc, test_integration_zero_handling);
    
    suite_add_tcase(s, tc);
    return s;
}

