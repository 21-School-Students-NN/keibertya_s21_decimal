#include "../include/s21_decimal.h"

/**
 * @brief Static helper function to emulate _addcarry_u32 intrinsic
 * @param carry_in Input carry flag (0 or 1)
 * @param x First 32-bit addend
 * @param y Second 32-bit addend
 * @param result Pointer to store the 32-bit sum
 * 
 * @return Output carry flag (0 or 1)
 */
static inline uint8_t adc_u32(uint8_t carry_in, uint32_t x, uint32_t y, uint32_t* result) {
    uint64_t sum = (uint64_t)x + y + (carry_in ? 1 : 0);
    *result = (uint32_t)sum;
    return (sum >> 32) & 1;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    *result = (s21_decimal){0};

    // TODO: handle different sign
    
    uint8_t carry = 0;
    for (int i = 0; i < 3; ++i)
        carry = adc_u32(carry, value_1.bits[i], value_2.bits[i], &result->bits[i]);
    
    result->sign = value_1.sign;
    
    if (carry) {
        return result->sign ? 2 : 1;
    }
    
    return 0;
}

s21_decimal dec = {1, 2, 3, 4};