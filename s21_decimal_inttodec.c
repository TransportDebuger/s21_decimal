#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int err = 1;

    if (dst) {
        for (char i = HIGH; i <= SERVICE; i++) {
            dst->bits[i] = 0;
        }
        if (src >= 0) {
            dst->bits[LOW] = src;
        } else {
            s21_set_bit(dst, SERVICE, SIGN_BIT);
            dst->bits[LOW] = -src;
        }
        err = 0;
    }

    return err;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int err= 1;
    if (dst) {
        s21_truncate(src, &src);
        if (!src.bits[HIGH] && !src.bits[MID] && src.bits[LOW] >= 0) {
            if (s21_get_sign(src)) {
                src.bits[LOW] = -(src.bits[LOW]);
            }
            *dst = src.bits[LOW];
            err = 0;
        }
    }
    return err
}