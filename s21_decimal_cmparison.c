#include "./s21_decimal.h"

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;
    
    //сравниваем знаки, так как дальнейшее вравнение при разности знаков не имеет смысла
    if (s21_get_sign(dec1) == s21_get_sign(dec2)) {
        //выравниваем числа и их порядки 
        int exp1 = s21_get_scale(dec1),
            exp2 = s21_get_scale(dec2);
        
        if (exp1 < exp2) {
            //выравниваем порядок dec1. Нужно раcширить функцию truncate
        } else if (exp1 > exp2) {
            //выравниваем порядок dec2. Нужно раcширить функцию truncate
        }
        //делаем побитовое сравнение частей decimal. 
        //В случае полного совпадения результатом XOR будет 0, иначе любое другое значение.
        if (!(dec1.bits[HIGH] ^ dec2.bits[HIGH]) && 
            !(dec1.bits[MID] ^ dec2.bits[MID]) && 
            !(dec1.bits[LOW] ^ dec2.bits[LOW])) {
                res = 1;
            }
    }

    return res;
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;

    if (s21_get_sign(dec1) < s21_get_sign(dec1)) {
        int exp1 = s21_get_scale(dec1),
            exp2 = s21_get_scale(dec2);
        
        if (exp1 < exp2) {
            //выравниваем порядок dec1. Нужно раcширить функцию truncate
        } else if (exp1 > exp2) {
            //выравниваем порядок dec2. Нужно раcширить функцию truncate
        }
        //опять лайфхак с union
        union numbits {
            int num;
            unsigned int nbit;
        };

        union numbits u_bits1 = {.num = dec1.bits[HIGH]}, 
                      u_bits2 = {.num = dec2.bits[HIGH]};
        
        //тупо сравниваем что меньше продвигаясь от старших к младши
        if (u_bits1.nbit < u_bits2.nbit) res = 1;
        else if (u_bits1.nbit == u_bits2.nbit) {
            u_bits1.num = dec1.bits[MID]; 
            u_bits2.num = dec2.bits[MID];
            if (u_bits1.nbit < u_bits2.nbit) res = 1;
            else if (u_bits1.nbit == u_bits2.nbit) {
                u_bits1.num = dec1.bits[LOW]; 
                u_bits2.num = dec2.bits[LOW];
                if (u_bits1.nbit < u_bits2.nbit) res = 1;
            }
        }
    }

    return res;
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;

    if (s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2)) res = 1;
    return res;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;

    if (!s21_is_less(dec2, dec1)) {
        res = 1;
    }

    return res;
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;

    if (s21_is_less(dec2, dec1) || s21_is_equal(dec1, dec2)) res = 1;
    
    return 0;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
    int res = 0;

    if (!(s21_is_equal(dec1, dec2))) res = 1;

    return res;
}