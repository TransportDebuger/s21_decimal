#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int err = 0;

    if (dst) {
        //знак
        if (src < 0) {
            s21_negate(*dst, dst);
            src *= -1;
        }
        //для облегчения работы с float как с набором бит
        union {
            float fvalue;
            int ivalue;
        } typeconvert;

        //вытаскиваем порядок
        typeconvert.fvalue = src; 
        int offset = (typeconvert.ivalue >> 23) << 16;
        dst->bits[0] |= offset;

        //вытаскиваем мантиссу
        dst->bits[3] = (typeconvert.ivalue << 9) >> 9;
    } else {
        err = 1;
    }

    return err;
}