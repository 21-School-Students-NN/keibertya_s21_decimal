#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <string.h>

#include "s21_decimal.h"

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

/*======================================================================
    MURK'S FUNCTIONS
======================================================================*/

/**
 * @brief Сравнивает абсолютные величины (мантиссы) двух чисел.
 * @return >0 если |value_1|>|value_2|, <0 если |value_1|<|value_2|, 0 если
 * равны.
 */
int32_t compare_mantissas_96(const s21_decimal value_1,
                             const s21_decimal value_2);

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
 * @brief Делит 97+ битное число на 10 с банковским округлением.
 * @param bits      Мантисса (будет изменена).
 * @param carry_in  Входящий перенос (старшие 32 бита числа).
 * @return uint32_t Возвращает новый перенос (0, если результат уместился в 96
 * бит).
 */
uint32_t divide_and_round(s21_decimal *num, uint32_t carry_in);

/**
 * @brief Нормализует два числа к общей степени "встречным" методом.
 *        Изменяет числа "по месту".
 * @return Код ошибки (0 - успех, 1 или 2 - ошибка).
 */
uint32_t normalizing(s21_decimal *num1, s21_decimal *num2);

/**
 * @brief Выполняет умножение 96-битных мантисс "в столбик".
 *
 * @param value_1 Первый множитель.
 * @param value_2 Второй множитель.
 * @param temp_result 192-битный (6 x uint32_t) массив для результата.
 */
void multiply_mantissas(s21_decimal value_1, s21_decimal value_2,
                        uint32_t temp_result[6]);

/**
 * @brief Нормализует 192-битный результат, чтобы он поместился в 96 бит.
 *
 * Функция делит 192-битное число на 10, уменьшая масштаб, пока оно не
 * поместится в 96 бит или пока масштаб не станет слишком маленьким. Применяет
 * банковское округление.
 *
 * @param temp_result 192-битный (6 x uint32_t) исходный результат.
 * @param scale Начальный масштаб.
 * @param sign Знак результата.
 * @param result Указатель на s21_decimal для записи финального значения.
 * @return int Код ошибки (0 - OK, 1 или 2 - переполнение).
 */
uint32_t normalize_and_fit(uint32_t temp_result[6], int scale, int sign,
                           s21_decimal *result);

#endif  // S21_HELPERS_H