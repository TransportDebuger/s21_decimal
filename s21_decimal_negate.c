#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
    int signbit = 1 << 31; //значение бита знака, соответственн 1 смещенная на 31 бит влево.
    int err = 0;

    if (result) {
        if (!(value.bits[0] & signbit)) result->bits[0] = value.bits[0] | signbit;
        else result->bits[0] = value.bits[0] & ~signbit;
    } else {
        err = 1;
    }

    return err;
}