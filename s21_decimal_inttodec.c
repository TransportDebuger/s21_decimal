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
        while (bitcount !=0) {
            int bitval = 0;
            if (src / 2) bitval = src % 2;
            dst->bits[3] = dst->bits[3] >> 1;
            dst->bits[3] = dst->bits[3] | getbit(dst->bits[2]);
            dst->bits[2] = dst->bits[2] >> 1;
            dst->bits[2] = dst->bits[2] | getbit(dst->bits[1]);
            dst->bits[1] = dst->bits[1] | (bitval << 31);
            --bitcount;
        }
    } else {
        err = 1;
    }

    return err;
}

int getbit(int src) {
    int bit = 1;

    return (src & bit) ? bit << 31 : 0;
}