#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  _init_decimal_zero(result);

  int num = value_1.bits[0] - value_2.bits[0];

  return num;
}