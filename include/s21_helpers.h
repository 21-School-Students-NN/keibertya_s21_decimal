#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <string.h>

#include "s21_decimal.h"

// de-comment for turning on the extended test
// #define ENABLE_EXTENDED_TESTS

#define SCALE_MASK 0x001F0000  // bit mask for scale bits 16-23 (8 bits) 0x1F
#define SIGN_MASK 0x80000000   // bit mask for sign bit 31

#define MAX_SCALE 28
#define MAX_SIGN 1

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

/**
 * @brief Check, does the decimal zero or not
 * @param dec Pointer to the decimal number to check
 * @return `0` if decimal NOT zero and `1` otherwise
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 05.09.2025
 */
int _is_decimal_zero(const s21_decimal *dec) __attribute__((nonnull));

/**
 * @brief Trying normalize decimal by multiply by 10
 * @param to_normalize Pointer to the decimal for normalize
 * @return `S21_SUCCESS` while normilized and `S21_ERROR` otherwise
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 15.08.2025
 */
int _normalize_to_upper(s21_decimal *to_normalize) __attribute__((nonnull));

/**
 * @brief Check, does the decimal zero or not
 * @param value_1 First pointer to the decimal for normalize
 * @param value_2 Second pointer to the decimal for normalize
 * @return `S21_SUCCESS` normalized successfully `S21_TOO_SMAll`,
 * `S21_TOO_LARGE` and `S21_ERROR` otherwise
 * @author Murk Mindrin x Demian Domozhirov (darkdomian@gmial.com | trelawnm at
 * 21 School)
 * @date 05.09.2025
 */
int _normalize(s21_decimal *value_1, s21_decimal *value_2)
    __attribute__((nonnull));

/**
 * @brief Normalize with rounding product of two decimal numbers and fit (if
 * possible) to the `result` decimal number
 * @param product The incoming production of two decimal to normalize for
 * @param scale The scale of decimal after product
 * @param sign The sign of decimal after product
 * @param result Pointer to result where store the normalized data
 * @return `S21_SUCCESS` on successful normalization, `S21_ERROR`,
 * `S21_TOO_LARGE` or `S21_TOO_SMALL` otherwise
 *
 * @author Murk Mindrin
 * @date 05.09.2025
 */
int _normalize_product(uint64_t product[6], int scale, int sign,
                       s21_decimal *result) __attribute__((nonnull));

/**
 * @brief Divide decimal by 10 with bank rounding
 * @param dec Pointer to the decimal witch one should be rounded.
 * @param carry Incoming carry data
 * @return Overflow in this case or zero if everything is fine
 * @author Murk Mindrin
 * @date 05.09.2025
 */
int _divide_and_round(s21_decimal *dec, uint32_t carry)
    __attribute__((nonnull));

/**
 * @brief Static helper function to emulate _addcarry_u32 intrinsic
 * @param x First 32-bit addend
 * @param y Second 32-bit addend
 * @param result Pointer to store the 32-bit sum
 *
 * @return Output carry flag (0 or 1)
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 05.09.2025
 */
int _add_with_carry(const s21_decimal *x, const s21_decimal *y,
                    s21_decimal *result) __attribute__((nonnull));

/**
 * @brief Abs compare for mantissas
 * @return `>0` while `|value_1|>|value_2|`, `<0` while `|value_1|<|value_2|`,
 * `0` if they are equal.
 * @author Mark Mindrin
 * @date 05.09.2025
 */
int _compare_mantissas(const s21_decimal *value_1, const s21_decimal *value_2);

/*======================================================================
    MURK'S FUNCTIONS
======================================================================*/

/**
 * @brief Складывает две 96-битные мантиссы.
 * @param a         Мантисса первого числа.
 * @param b         Мантисса второго числа.
 * @param result    Указатель для записи 96-битного результата.
 * @return uint32_t Возвращает 32-битный перенос (carry_out).
 */
uint32_t add_96_mantissas(const s21_decimal num1, const s21_decimal num2,
                          s21_decimal *result);

/**
 * @brief Увеличивает мантиссу s21_decimal на 1. (Ваша версия)
 * @param num Указатель на s21_decimal для инкремента.
 * @return 1 при переполнении, 0 при успехе.
 */
uint8_t increment_96(s21_decimal *num);

/**
 * @brief Умножает 96-битную мантиссу на 10.
 * @param bits Мантисса для умножения (будет изменена).
 * @return true при переполнении, иначе false.
 */
int multiply_96_mantissa(s21_decimal *num);

/**
 * @brief Выполняет умножение 96-битных мантисс "в столбик".
 *
 * @param value_1 Первый множитель.
 * @param value_2 Второй множитель.
 * @param temp_result 192-битный (6 x uint32_t) массив для результата.
 */
void multiply_mantissas(s21_decimal value_1, s21_decimal value_2,
                        uint32_t temp_result[6]);

#endif  // S21_HELPERS_H