#ifndef S21_HALPERS_FUNC_H
#define S21_HALPERS_FUNC_H
#include "s21_decimal.h"
int compare_mantissas_96(const s21_decimal value_1, const s21_decimal value_2);
uInt add_96_mantissas(const s21_decimal num1, const s21_decimal num2,
                      s21_decimal *result);
uByte increment_96(s21_decimal *num);
bool multiply_96_mantissa(s21_decimal *num);
uInt divide_and_round(s21_decimal *num, uInt carry_in);
int normalizing(s21_decimal *num1, s21_decimal *num2);

#endif