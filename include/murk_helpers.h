#ifndef S21_HALPERS_FUNC_H
#define S21_HALPERS_FUNC_H
#include "murk_decimal.h"
Int compare_mantissas_96(const s21_decimal value_1, const s21_decimal value_2);
uInt add_96_mantissas(const s21_decimal num1, const s21_decimal num2,
                      s21_decimal *result);
uByte increment_96(s21_decimal *num);
bool multiply_96_mantissa(s21_decimal *num);
uInt divide_and_round(s21_decimal *num, uInt carry_in);
uInt normalizing(s21_decimal *num1, s21_decimal *num2);
void multiply_mantissas(s21_decimal value_1, s21_decimal value_2,
                        uint32_t temp_result[6]);
uInt normalize_and_fit(uint32_t temp_result[6], int scale, int sign,
                       s21_decimal *result);
#endif