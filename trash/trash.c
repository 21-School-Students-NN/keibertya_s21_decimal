/*
uByte devide_96_by_10(s21_decimal *num) {
    s21_decimal tmp_num = *num;
    int current_scale = GET_SCALE(tmp_num);
    uByte error_fl = 0;
    if(current_scale > 0) {
        SET_SCALE(&tmp_num, GET_SCALE(tmp_num));

        uInt quotient[3] = {0, 0, 0};
        uLong temp_carry = 0;
        for (int i = 2; i >= 0; i--) {
            temp_carry = (temp_carry << 32) | tmp_num.bits[i];
            quotient[i] = (uInt)(temp_carry / 10);
            temp_carry %= 10;
        }
        uInt remainder = (uInt)temp_carry;

        bool round_up = false;
        if (remainder > 5 || (remainder == 5 && (quotient[0] & 1))) {
            round_up = true;
        }

        if (round_up) {
            uInt temp_96[3] = {quotient[0], quotient[1], quotient[2]};
            if (increment_96(quotient)) { 
                error_fl = S21_INFINITY;
            } else{
                tmp_num.bits[0] = temp_96[0];
                tmp_num.bits[1] = temp_96[1];
                tmp_num.bits[2] = temp_96[2];
                *num = tmp_num;
            }
        }
    } else {
        error_fl = S21_INFINITY;
    }
    return error_fl;
}*/

/*uByte iterative_scale_down(s21_decimal *num, uInt carry_in) {
    uInt current_mantissa[4] = {num->bits[0], num->bits[1], num->bits[2], carry_in};
    int current_scale = GET_SCALE(*num);

    while (current_mantissa[3] > 0) {
        
        if (current_scale <= 0) {
            return S21_INFINITY; 
        }
        
        current_scale--;

        uInt quotient[4] = {0, 0, 0, 0};
        uLong temp_carry = 0;
        for (int i = 3; i >= 0; i--) {
            temp_carry = (temp_carry << 32) | current_mantissa[i];
            quotient[i] = (uInt)(temp_carry / 10);
            temp_carry %= 10;
        }
        uInt remainder = (uInt)temp_carry;

        bool round_up = false;
        if (remainder > 5 || (remainder == 5 && (quotient[0] & 1))) {
            round_up = true;
        }

        if (round_up) {
            uInt temp_96[3] = {quotient[0], quotient[1], quotient[2]};
            if (increment_96(temp_96)) { 
                quotient[3]++; 
            }
            quotient[0] = temp_96[0];
            quotient[1] = temp_96[1];
            quotient[2] = temp_96[2];
        }
        
        current_mantissa[0] = quotient[0];
        current_mantissa[1] = quotient[1];
        current_mantissa[2] = quotient[2];
        current_mantissa[3] = quotient[3];
    }

    num->bits[0] = current_mantissa[0];
    num->bits[1] = current_mantissa[1];
    num->bits[2] = current_mantissa[2];
    SET_SCALE(num, current_scale);

    return S21_SUCCESS;
}*/