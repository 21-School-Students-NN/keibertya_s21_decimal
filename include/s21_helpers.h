#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "s21_decimal.h"

#define SCALE_MASK 0x001F0000 // bit mask for scale bits 16-23 (8 bits) 0x1C
#define SIGN_MASK 0x80000000 // bit mask for sign bit 31

/**
 * @brief Gets the scale (exponent) from a decimal number
 * @param dec Pointer to the decimal number
 * @return Scale value (0-28) as unsigned char, or `HELPERS_ERROR` if dec is NULL
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
unsigned char s21_get_scale(const s21_decimal *dec);

/**
 * @brief Sets the scale (exponent) of a decimal number
 * @param dec Pointer to the decimal number to modify
 * @param scale Scale value to set (0-28)
 * @return 0 on success, `HELPERS_ERROR` on error (invalid scale or NULL pointer)
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
char s21_set_scale(s21_decimal *dec, const unsigned char scale);

/**
 * @brief Gets the sign from a decimal number
 * @param dec Pointer to the decimal number
 * @return Sign value (0 for positive, 1 for negative) as unsigned char, or `HELPERS_ERROR` if dec is NULL
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
unsigned char s21_get_sign(const s21_decimal *dec);

/**
 * @brief Sets the sign of a decimal number
 * @param dec Pointer to the decimal number to modify
 * @param sign Sign value to set (0 for positive, 1 for negative)
 * @return 0 on success, `HELPERS_ERROR` on error (invalid sign or NULL pointer)
 * @author Demian Domozhirov (darkdomian@gmial.com | trelawnm at 21 School)
 * @date 29.07.2025
 */
char s21_set_sign(s21_decimal *dec, const unsigned char sign);

#endif // S21_HELPERS_H