/**
 * @file s21_decimal.h
 * @brief Decimal arithmetic library implementation
 */

#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdint.h>

/** @brief Structure to represent decimal numbers with a mantissa and exponent
 */
typedef struct {
  uint32_t bits[4];
} s21_decimal;

/*======================================================================
    STATUS CODE DEFINITIONS
======================================================================*/

/** @brief Operation completed successfully */
#define S21_SUCCESS 0
/** @brief Generic error code */
#define S21_ERROR 1

/** @brief Number is too large or equal to infinity */
#define S21_TOO_LARGE 1
/** @brief Number is too small or equal to negative infinity */
#define S21_TOO_SMALL 2
/** @brief Division by zero encountered */
#define S21_DIV_BY_ZERO 3

/** @brief `False` value for comparison operations */
#define S21_FALSE 0
/** @brief `True` value for comparison operations */
#define S21_TRUE 1

/*======================================================================
    ARITHMETIC OPERATORS
======================================================================*/

/**
 * @brief Adds two decimal numbers
 * @param value_1 First addend
 * @param value_2 Second addend
 * @param result Pointer to store the sum
 * @return Error code: `0` (OK), `1` (too large), `2` (too small), `3` (division
 * by zero)
 * @author
 * @date
 */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/**
 * @brief Subtracts two decimal numbers
 * @param value_1 Minuend
 * @param value_2 Subtrahend
 * @param result Pointer to store the difference
 * @return Error code: `0` (OK), `1` (too large), `2` (too small), `3` (division
 * by zero)
 * @author
 * @date
 */
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/**
 * @brief Multiplies two decimal numbers
 * @param value_1 First factor
 * @param value_2 Second factor
 * @param result Pointer to store the product
 * @return Error code: `0` (OK), `1` (too large), `2` (too small), `3` (division
 * by zero)
 * @author
 * @date
 */
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/**
 * @brief Divides two decimal numbers
 * @param value_1 Dividend
 * @param value_2 Divisor
 * @param result Pointer to store the quotient
 * @return Error code: `0` (OK), `1` (too large), `2` (too small), `3` (division
 * by zero)
 * @author
 * @date
 */
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/*======================================================================
    COMPARISON OPERATORS
======================================================================*/

/**
 * @brief Checks if `value_1` is less than `value_2`
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_less(s21_decimal value_1, s21_decimal value_2);

/**
 * @brief Checks if `value_1` is less than or equal to `value_2`
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);

/**
 * @brief Checks if `value_1` is greater than `value_2`
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);

/**
 * @brief Checks if `value_1` is greater than or equal to `value_2`
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);

/**
 * @brief Checks if two decimal numbers are equal
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);

/**
 * @brief Checks if two decimal numbers are not equal
 * @param value_1 First decimal number to compare
 * @param value_2 Second decimal number to compare
 * @return Comparison result: `0` (FALSE), `1` (TRUE)
 * @author
 * @date
 */
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

/*======================================================================
    CONVERTORS AND PARSERS
======================================================================*/

/**
 * @brief Converts from int to decimal
 * @param src Integer to convert
 * @param dst Pointer to store the result
 * @return Error code: `0` (OK), `1` (conversion error)
 * @author
 * @date
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst);

/**
 * @brief Converts from float to decimal
 * @param src Float to convert
 * @param dst Pointer to store the result
 * @return Error code: `0` (OK), `1` (conversion error)
 * @author
 * @date
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst);

/**
 * @brief Converts from decimal to int
 * @param src Decimal to convert
 * @param dst Pointer to store the result
 * @return Error code: `0` (OK), `1` (conversion error)
 * @author
 * @date
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst);

/**
 * @brief Converts from decimal to float
 * @param src Decimal to convert
 * @param dst Pointer to store the result
 * @return Error code: `0` (OK), `1` (conversion error)
 * @author
 * @date
 */
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/*======================================================================
    OTHER FUNCTIONS
======================================================================*/

/**
 * @brief Rounds down to the nearest integer
 * @param value Decimal number to round
 * @param result Pointer to store the rounded value
 * @return Error code: `0` (OK), `1` (error)
 * @author
 * @date
 */
int s21_floor(s21_decimal value, s21_decimal *result);

/**
 * @brief Rounds to the nearest integer
 * @param value Decimal number to round
 * @param result Pointer to store the rounded value
 * @return Error code: `0` (OK), `1` (error)
 * @author Amfir (s21: tyananai)
 * @date April 18, 2025
 */
int s21_round(s21_decimal value, s21_decimal *result);

/**
 * @brief Truncates decimal digits (rounds toward zero)
 * @param value Decimal number to truncate
 * @param result Pointer to store the truncated value
 * @return Error code: `0` (OK), `1` (error)
 * @author
 * @date
 */
int s21_truncate(s21_decimal value, s21_decimal *result);

/**
 * @brief Negates the decimal number
 * @param value Decimal number to negate
 * @param result Pointer to store the negated value
 * @return Error code: `0` (OK), `1` (error)
 * @author
 * @date
 */
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // S21_DECIMAL_H