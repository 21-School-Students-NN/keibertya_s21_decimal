#ifndef S21_HALPERS_FUNC_H
#define S21_HALPERS_FUNC_H
#include "s21_decimal.h"
int32_t compare_mantissas_96(const s21_decimal value_1, const s21_decimal value_2);
uint32_t add_96_mantissas(const s21_decimal num1, const s21_decimal num2,
                      s21_decimal *result);
uint8_t increment_96(s21_decimal *num);
int multiply_96_mantissa(s21_decimal *num);
uint32_t divide_and_round(s21_decimal *num, uint32_t carry_in);
uint32_t normalizing(s21_decimal *num1, s21_decimal *num2);
void multiply_mantissas(s21_decimal value_1, s21_decimal value_2,
                        uint32_t temp_result[6]);
uint32_t normalize_and_fit(uint32_t temp_result[6], int scale, int sign,
                       s21_decimal *result);
#endif