#include "s21_decimal.h"

Int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  uByte error_fl = 0;
  if (GET_SCALE(value_1) == GET_SCALE(value_2) &&
      GET_SIGN(value_1) == GET_SIGN(value_2)) {
    error_fl = naive_addition_with_result_processing(value_1, value_2, &result);
  } else if (GET_SCALE(value_1) != GET_SCALE(value_2)) {
    if (!normalizing(&value_1, &value_2)) {
      if (GET_SIGN(value_1) == GET_SIGN(value_2)) {
        error_fl =
            naive_addition_with_result_processing(value_1, value_2, result);
      } else {
        s21_decimal high_num;
        s21_decimal low_num;
        s21_decimal uRes;
        if (compare_mantissas_96(value_1, value_2) >= 0) {
          high_num = value_1;
          low_num = value_2;
        } else {
          high_num = value_2;
          low_num = value_1;
        }
        low_num.bits[0] = ~low_num.bits[0];
        low_num.bits[1] = ~low_num.bits[1];
        low_num.bits[2] = ~low_num.bits[2];
        increment_96(&low_num);

        naive_addition_with_result_processing(high_num, low_num, &uRes);
        uRes.bits[3] = high_num.bits[3];
        *result = uRes;
      }
    }
  }
  return error_fl;
}

uByte naive_addition_with_result_processing(s21_decimal value_1,
                                            s21_decimal value_2,
                                            s21_decimal *result) {
  uByte error_fl = S21_SUCCESS;
  uLong low_sum = (uLong)value_1.bits[0] + value_2.bits[0];
  uLong mid_sum = (uLong)value_1.bits[1] + value_2.bits[1] + (low_sum << 32);
  uLong high_sum = (uLong)value_1.bits[2] + value_2.bits[2] + (mid_sum << 32);
  if ((high_sum >> 32) == 0) {
    result->bits[0] = low_sum;
    result->bits[1] = mid_sum;
    result->bits[2] = high_sum;
    result->bits[3] = value_1.bits[3];
  } else {
    s21_decimal overflowed_result;
    overflowed_result.bits[0] = (uInt)low_sum;
    overflowed_result.bits[1] = (uInt)mid_sum;
    overflowed_result.bits[2] = (uInt)high_sum;
    overflowed_result.bits[3] = value_1.bits[3];
    uInt carry_in = (uInt)(high_sum >> 32);
    while (carry_in > 0) {
      if (devide_96_by_10(&overflowed_result, &carry_in)) {
        error_fl = S21_INFINITY;
        break;
      }
    }
    *result = overflowed_result;
  }
  return error_fl;
}

uInt compare_mantissas_96(const s21_decimal num1, const s21_decimal num2) {
  uInt fl = 0;
  for (int i = 2; i >= 0; i--) {
    if (num1.bits[i] > num2.bits[i]) {
      fl = 1;
      break;
    }
    if (num1.bits[i] < num2.bits[i]) {
      fl = -1;
      break;
    }
  }
  return fl;
}

uByte normalizing(s21_decimal *num1, s21_decimal *num2) {
  s21_decimal tmp_high;
  s21_decimal tmp_low;
  uByte error_fl = S21_SUCCESS;
  if (GET_SCALE(*num1) < GET_SCALE(*num2)) {
    tmp_low = *num1;
    tmp_high = *num2;
  } else {
    tmp_low = *num2;
    tmp_high = *num1;
  }
  while (GET_SCALE(tmp_low) != GET_SCALE(tmp_high)) {
    uByte carry_mult = multiply_96_by_10(&tmp_low);
    if (carry_mult) {
      uInt plug = 0;
      uByte carrydev = devide_96_by_10(&tmp_high, &plug);
      if (carrydev) {
        error_fl = S21_INFINITY;
        break;
      }
    }
  }
  if (!error_fl) {
    if (GET_SCALE(*num1) < GET_SCALE(*num2)) {
      *num1 = tmp_low;
      *num2 = tmp_high;
    } else {
      *num2 = tmp_low;
      *num1 = tmp_high;
    }
  }
  return error_fl;
}

uByte multiply_96_by_10(s21_decimal *num) {
  uLong carry = 0;
  s21_decimal tmp_num = *num;
  uInt x[3] = {tmp_num.bits[0], tmp_num.bits[1], tmp_num.bits[2]};

  uInt x2[3];
  carry = (uLong)x[0] << 1;
  x2[0] = (uInt)carry;
  carry = ((uLong)x[1] << 1) | (carry >> 32);
  x2[1] = (uInt)carry;
  carry = ((uLong)x[2] << 1) | (carry >> 32);
  x2[2] = (uInt)carry;
  if (carry >> 32) return true;

  uInt x8[3];
  carry = (uLong)x[0] << 3;
  x8[0] = (uInt)carry;
  carry = ((uLong)x[1] << 3) | (carry >> 32);
  x8[1] = (uInt)carry;
  carry = ((uLong)x[2] << 3) | (carry >> 32);
  x8[2] = (uInt)carry;
  if (carry >> 32) return true;

  uLong sum = (uLong)x2[0] + x8[0];
  tmp_num.bits[0] = (uInt)sum;
  sum = (uLong)x2[1] + x8[1] + (sum >> 32);
  tmp_num.bits[1] = (uInt)sum;
  sum = (uLong)x2[2] + x8[2] + (sum >> 32);
  tmp_num.bits[2] = (uInt)sum;
  SET_SCALE(&tmp_num, GET_SCALE(tmp_num) + 1);
  if ((sum >> 32) == 0) {
    *num = tmp_num;
  }
  return (sum >> 32) != 0;
}

uByte devide_96_by_10(s21_decimal *num, uInt *carry_in) {
  s21_decimal tmp_num = *num;
  uInt current_scale = GET_SCALE(tmp_num);
  uInt tmp_info_byte;
  if (*carry_in) {
    tmp_info_byte = tmp_num.bits[3];
    tmp_num.bits[3] = carry_in;
  }
  uByte error_fl = S21_SUCCESS;
  if (current_scale > 0) {
    uLong temp_carry = 0;
    for (int i = (*carry_in) ? 3 : 2; i >= 0; i--) {
      temp_carry = (temp_carry << 32) | tmp_num.bits[i];
      tmp_num.bits[i] = (uInt)(temp_carry / 10);
      temp_carry %= 10;
    }

    uInt remainder = (uInt)temp_carry;

    bool round_up = false;

    if (remainder > 5 || (remainder == 5 && (tmp_num.bits[0] & 1))) {
      round_up = true;
    }

    if (round_up) {
      if (increment_96(&tmp_num)) {
        error_fl = S21_INFINITY;
      }
    }
  } else {
    error_fl = S21_INFINITY;
  }
  if (*carry_in) {
    tmp_num.bits[3] = tmp_info_byte;
  }
  SET_SCALE(&tmp_num, GET_SCALE(tmp_num) - 1);
  if (!error_fl) {
    *num = tmp_num;
    *carry_in = tmp_num.bits[3];
  }
  return error_fl;
}

uByte increment_96(s21_decimal *num) {
  uInt tmp_num[words - 1];
  tmp_num[0] = num->bits[0];
  tmp_num[1] = num->bits[1];
  tmp_num[2] = num->bits[2];

  uByte error_fl = S21_SUCCESS;

  uLong summ = (uLong)tmp_num[0] + 1;
  tmp_num[0] = (uInt)summ;

  if ((uInt)(summ >> 32)) {
    summ = (uLong)tmp_num[1] + 1;
    tmp_num[1] = (uInt)summ;

    if ((uInt)(summ >> 32)) {
      summ = (uLong)tmp_num[2] + 1;
      tmp_num[2] = (uInt)summ;

      if ((uInt)(summ >> 32)) {
        error_fl = 1;
      }
    }
  }
  if (!error_fl) {
    num->bits[0] = tmp_num[0];
    num->bits[1] = tmp_num[1];
    num->bits[2] = tmp_num[2];
  }
  return error_fl;
}