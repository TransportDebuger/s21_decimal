#include "s21_decimal.h"

int getbit(int src);

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int err = 0;

    if (dst) {
        int bitcount = 95;

        for (char i = 0; i < 4; i++) dst->bits[i] = 0;
        if (src < 0) {
            s21_negate(*dst, dst);
            src *= -1;
        }
        dst->bits[3] = src;
    } else {
        err = 1;
    }

    return err;
}
/*
int getbit(int src) {
    int bit = 1;

    return (src & bit) ? bit << 31 : 0;
}*/