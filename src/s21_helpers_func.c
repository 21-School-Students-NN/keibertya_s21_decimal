#include "../include/s21_decimal.h"
// =================================================================================
//                    ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (НИЗКИЙ УРОВЕНЬ)
// =================================================================================

static int compare_mantissas_96(const s21_decimal value_1, const s21_decimal value_2);
static uInt add_96_mantissas(const s21_decimal num1, const s21_decimal num2, s21_decimal *result);
uByte increment_96(s21_decimal *num);
static bool multiply_96_mantissa(s21_decimal *num);
static uInt divide_and_round(s21_decimal *num, uInt carry_in);
static int normalizing(s21_decimal *num1, s21_decimal *num2);

/**
 * @brief Сравнивает абсолютные величины (мантиссы) двух чисел.
 * @return >0 если |value_1|>|value_2|, <0 если |value_1|<|value_2|, 0 если равны.
 */
static int compare_mantissas_96(const s21_decimal value_1, const s21_decimal value_2) {
    for (int i = 2; i >= 0; i--) {
        if (value_1.bits[i] > value_2.bits[i]) return 1;
        if (value_1.bits[i] < value_2.bits[i]) return -1;
    }
    return 0;
}

/**
 * @brief Складывает две 96-битные мантиссы.
 * @param a         Мантисса первого числа.
 * @param b         Мантисса второго числа.
 * @param result    Указатель для записи 96-битного результата.
 * @return uInt Возвращает 32-битный перенос (carry_out).
 */
static uInt add_96_mantissas(const s21_decimal num1, const s21_decimal num2, s21_decimal *result) {
    uLong sum = (uLong)num1.bits[0] + num2.bits[0];
    result->bits[0] = (uInt)sum;
    sum = (uLong)num1.bits[1] + num2.bits[1] + (sum >> 32);
    result->bits[1] = (uInt)sum;
    sum = (uLong)num1.bits[2] + num2.bits[2] + (sum >> 32);
    result->bits[2] = (uInt)sum;
    return (uInt)(sum >> 32);
}

/**
 * @brief Увеличивает мантиссу s21_decimal на 1. (Ваша версия)
 * @param num Указатель на s21_decimal для инкремента.
 * @return 1 при переполнении, 0 при успехе.
 */
uByte increment_96(s21_decimal *num){
    uInt tmp_num[3];
    tmp_num[0] = num->bits[0];
    tmp_num[1] = num->bits[1];
    tmp_num[2] = num->bits[2];
    uByte error_fl = 0;
    uLong summ = (uLong)tmp_num[0] + 1;
    tmp_num[0] = (uInt)summ;
    if((uInt)(summ>>32)){
        summ = (uLong)tmp_num[1] + 1;
        tmp_num[1] = (uInt)summ;
        if((uInt)(summ>>32)){
            summ = (uLong)tmp_num[2] + 1;
            tmp_num[2] = (uInt)summ;
            if((uInt)(summ>>32)) error_fl = 1;
        }
    }
    if(!error_fl) {
        num->bits[0] = tmp_num[0];
        num->bits[1] = tmp_num[1];
        num->bits[2] = tmp_num[2];
    }
    return error_fl;
}

/**
 * @brief Умножает 96-битную мантиссу на 10.
 * @param bits Мантисса для умножения (будет изменена).
 * @return true при переполнении, иначе false.
 */
static bool multiply_96_mantissa(s21_decimal *num) {
    uInt x[3] = {num->bits[0], num->bits[1], num->bits[2]};
    // x*2
    uInt x2[3];
    uLong carry = (uLong)x[0] << 1; x2[0] = (uInt)carry;
    carry = ((uLong)x[1] << 1) | (carry >> 32); x2[1] = (uInt)carry;
    carry = ((uLong)x[2] << 1) | (carry >> 32); x2[2] = (uInt)carry;
    if (carry >> 32) return true;
    // x*8
    uInt x8[3];
    carry = (uLong)x[0] << 3; x8[0] = (uInt)carry;
    carry = ((uLong)x[1] << 3) | (carry >> 32); x8[1] = (uInt)carry;
    carry = ((uLong)x[2] << 3) | (carry >> 32); x8[2] = (uInt)carry;
    if (carry >> 32) return true;
    // x*10 = x*2 + x*8
    s21_decimal tmp_dec_x2 = {x2[0], x2[1], x2[2], 0};
    s21_decimal tmp_dec_x8 = {x8[0], x8[1], x8[2], 0};
    return add_96_mantissas(tmp_dec_x2, tmp_dec_x8, num) > 0;
}

/**
 * @brief Делит 97+ битное число на 10 с банковским округлением.
 * @param bits      Мантисса (будет изменена).
 * @param carry_in  Входящий перенос (старшие 32 бита числа).
 * @return uInt Возвращает новый перенос (0, если результат уместился в 96 бит).
 */
static uInt divide_and_round(s21_decimal *num, uInt carry_in) {
    uInt dividend[4] = {num->bits[0], num->bits[1], num->bits[2], carry_in};
    uInt quotient[4] = {0, 0, 0, 0};
    uLong temp_carry = 0;

    for (int i = 3; i >= 0; i--) {
        temp_carry = (temp_carry << 32) | dividend[i];
        quotient[i] = (uInt)(temp_carry / 10);
        temp_carry %= 10;
    }

    if (temp_carry > 5 || (temp_carry == 5 && (quotient[0] & 1))) {
        s21_decimal temp_dec = {{quotient[0], quotient[1], quotient[2], 0}};
        if(increment_96(&temp_dec)){
            quotient[3]++;
        }
        quotient[0] = temp_dec.bits[0];
        quotient[1] = temp_dec.bits[1];
        quotient[2] = temp_dec.bits[2];
    }
    
    num->bits[0] = quotient[0];
    num->bits[1] = quotient[1];
    num->bits[2] = quotient[2];
    return quotient[3];
}

/**
 * @brief Нормализует два числа к общей степени "встречным" методом.
 *        Изменяет числа "по месту".
 * @return Код ошибки (0 - успех, 1 или 2 - ошибка).
 */
static int normalizing(s21_decimal *num1, s21_decimal *num2) {
    s21_decimal *ptr_low_scale, *ptr_high_scale;
    if (GET_SCALE(*num1) < GET_SCALE(*num2)) {
        ptr_low_scale = num1; ptr_high_scale = num2;
    } else {
        ptr_low_scale = num2; ptr_high_scale = num1;
    }

    while (GET_SCALE(*ptr_low_scale) != GET_SCALE(*ptr_high_scale)) {
        if (!multiply_96_mantissa(ptr_low_scale->bits)) {
            SET_SCALE(ptr_low_scale, GET_SCALE(*ptr_low_scale) + 1);
        } else {
            if (GET_SCALE(*ptr_high_scale) == 0) {
                return GET_SIGN(*ptr_low_scale) ? S21_NEGATIVE_INFINITY : S21_INFINITY;
            }
            uInt carry_zero = 0;
            divide_and_round(ptr_high_scale->bits, carry_zero);
            SET_SCALE(ptr_high_scale, GET_SCALE(*ptr_high_scale) - 1);
        }
    }
    return S21_SUCCESS;
}