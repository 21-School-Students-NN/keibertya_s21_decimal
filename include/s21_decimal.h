#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define S21_SUCCESS 0
#define S21_INFINITY 1
#define S21_NEGATIVE_INFINITY 2

#include <stdbool.h>
#include <stdint.h>

#define Byte int8_t
#define uByte uint8_t
#define Short int16_t
#define uShort uint16_t
#define Int int32_t
#define uInt uint32_t
#define Long int64_t
#define uLong uint64_t

#define words 4

#define GET_SIGN(d) ((d).bits[3] >> 31)
#define SET_SIGN(d_ptr, sign)          \
  do {                                 \
    if (sign) {                        \
      (d_ptr)->bits[3] |= (1U << 31);  \
    } else {                           \
      (d_ptr)->bits[3] &= ~(1U << 31); \
    }                                  \
  } while (0)

// #define GET_SCALE(d) (((d).bits[3] >> 16) & 0xFF)
// #define SET_SCALE(d_ptr, scale)
//   do {
//     ((d_ptr)->bits[3]) &= ~(0xFFU << 16);
//     (d_ptr)->bits[3] |= (((uInt)(scale) & 0xFFU) << 16);
//   } while (0)

#define GET_SCALE(d) (((uint32_t)(d).bits[3] >> 16) & 0xFFu)

#define SET_SCALE(d_ptr, scale)                  \
  do {                                           \
    uint32_t _b3 = (uint32_t)((d_ptr)->bits[3]); \
    _b3 &= ~(0xFFu << 16);                       \
    _b3 |= (((uint32_t)(scale) & 0xFFu) << 16);  \
    (d_ptr)->bits[3] = (int)_b3;                 \
  } while (0)

typedef struct {
  uInt bits[words];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
uInt s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
#endif