#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <string.h>
#include <stdio.h>

#include "s21_decimal.h"

#define SCALE_MASK 0x001F0000  // bit mask for scale bits 16-23 (8 bits) 0x1F
#define SIGN_MASK 0x80000000   // bit mask for sign bit 31

#define MAX_SCALE 28
#define MAX_SIGN 1

#define MAX_PRECISION 29
#define DECIMAL_MANTISSA_BITS 96

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
 * @brief Initialize decimal with zero
 * @param dec Pointer to the decimal number to initialize
 * @return nothing
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 02.08.2025
 */
void _init_decimal_zero(s21_decimal *dec) __attribute__((nonnull));
int _is_zero(s21_decimal value) __attribute__((nonnull));
int _is_equal(s21_decimal value1, s21_decimal value2);
void _print_decimal_debug(s21_decimal dec);
//======================================================================
//  Mantissa helpers (96-bit) and normalization
//======================================================================

int _compare_mantissas_96(const s21_decimal *a, const s21_decimal *b)
    __attribute__((nonnull));

uint32_t _add_mantissas_96(const s21_decimal *a, const s21_decimal *b,
                           s21_decimal *sum) __attribute__((nonnull));

int _multiply_mantissas_96(const s21_decimal *a, const s21_decimal *b,
                           uint32_t out192[6]) __attribute__((nonnull));

uint32_t _divide_by_10_with_bank_round(s21_decimal *num, uint32_t carry_in)
    __attribute__((nonnull));

int _normalize_scales_meet(s21_decimal *lower_scale, s21_decimal *higher_scale)
    __attribute__((nonnull));

int _normalize_and_fit_192_to_96(uint32_t temp192[6], meta_t *scale,
                                 meta_t sign, s21_decimal *result)
    __attribute__((nonnull));

#endif  // S21_HELPERS_H