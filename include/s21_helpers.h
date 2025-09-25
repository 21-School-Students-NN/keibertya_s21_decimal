#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

// de-comment for turning on the extended test
// #define ENABLE_EXTENDED_TESTS

#define SCALE_MASK 0x001F0000  // bit mask for scale bits 16-23 (8 bits) 0x1F
#define SIGN_MASK 0x80000000   // bit mask for sign bit 31

#define MAX_SCALE 28
#define MAX_SIGN 1

#define MAX_PRECISION 29
#define DECIMAL_MANTISSA_BITS 96

typedef struct {
  uint32_t bits[6];
} s21_uint192_t;

/** @brief Data type for holding the meta information about decimal such `sign`
 * or `scale` */
typedef unsigned char meta_t;

/**
 * @brief Gets the scale (exponent) from a decimal number
 * @param dec Pointer to the decimal number (can't be `NULL`)
 * @return Scale value (0-31) as `meta_t`
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
meta_t _get_scale(const s21_decimal *dec) __attribute__((nonnull));

/**
 * @brief Sets the scale (exponent) of a decimal number
 * @param dec Pointer to the decimal number to modify (can't be `NULL`)
 * @param scale Scale value to set (0-28)
 * @return `S21_SUCCESS` on success, `S21_ERROR` on error (invalid scale)
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
int _set_scale(s21_decimal *dec, const meta_t scale) __attribute__((nonnull));

/**
 * @brief Gets the sign from a decimal number
 * @param dec Pointer to the decimal number (can't be `NULL`)
 * @return Sign value (0 for positive, 1 for negative) as `meta_t`
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
meta_t _get_sign(const s21_decimal *dec) __attribute__((nonnull));

/**
 * @brief Sets the sign of a decimal number
 * @param dec Pointer to the decimal number to modify (can't be `NULL`)
 * @param sign Sign value to set (0 for positive, 1 for negative)
 * @return `S21_SUCCESS` on success, `S21_ERROR` on error (invalid sign)
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
int _set_sign(s21_decimal *dec, const meta_t sign) __attribute__((nonnull));

/**
 * @brief initializes decimal mantissa, scale and sign separately
 * @param low represents bit[0]
 * @param mid represents bit[1]
 * @param high represents bit[2]
 * @param scale represents scale of result
 * @param sign sign of result
 * @return s21_decimal
 * @author Amfir (s21: tyananai)
 */
s21_decimal make_decimal(uint32_t low, uint32_t mid, uint32_t high,
                         uint32_t scale, uint32_t sign);

/**
 * @brief Initialize decimal with zero
 * @param dec Pointer to the decimal number to initialize
 * @return nothing
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 02.08.2025
 */
void _init_decimal_zero(s21_decimal *dec) __attribute__((nonnull));

/**
 * @brief Check, does the decimal zero or not
 * @param dec Pointer to the decimal number to check
 * @return `0` if decimal NOT zero and `1` otherwise
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 05.09.2025
 */
int _is_decimal_zero(const s21_decimal *dec) __attribute__((nonnull));

/*======================================================================
    MURK'S FUNCTIONS
======================================================================*/

int _multiply_by_10(s21_decimal *value);

uint32_t _divide_by_10(s21_decimal *value, uint32_t remainder);

/**
 * @brief Gets single bit from mantissa
 * @param dec Pointer to the decimal number to initialize
 * @param order order (from 0 to 95)
 * @return bit value (0 for positive, 1 for negative) as `meta_t`
 * @author Anton Gashturi (bernieer)
 * @date 14.08.2025
 */
meta_t _get_bit(s21_decimal *dec, unsigned order) __attribute__((nonnull));

/**
 * @brief 192-bit arithmetic
 * @author Anton Gashturi (bernieer)
 */

void from_decimal_to_int192(s21_decimal value, s21_uint192_t *result);

int uint192_compare(s21_uint192_t value1, s21_uint192_t value2);

int uint192_shift_left(s21_uint192_t *value, uint32_t shift);

void uint192_shift_right(s21_uint192_t *value, uint32_t shift);

uint32_t uint192_add(s21_uint192_t value1, s21_uint192_t value2,
                     s21_uint192_t *result);

int uint192_sub(s21_uint192_t value1, s21_uint192_t value2,
                s21_uint192_t *result);

int uint192_div(s21_uint192_t dividend, s21_uint192_t divisor,
                s21_uint192_t *quotient, s21_uint192_t *remainder);

int leveling(s21_decimal value_1, s21_decimal value_2, s21_uint192_t *res1,
             s21_uint192_t *res2);

int uint192_mult_by_10(s21_uint192_t *value1);

uint32_t uint192_div_by_10(s21_uint192_t *value);

int leveling(s21_decimal value_1, s21_decimal value_2, s21_uint192_t *res1,
             s21_uint192_t *res2);

int uint192_is_not_zero(s21_uint192_t *value);

int from_uint192_to_decimal(s21_uint192_t *src, meta_t scale, s21_decimal *dst);

#endif  // S21_HELPERS_H