#ifndef S21_DECIMAL_COMMON_H
#define S21_DECIMAL_COMMON_H

#include "./s21_decimal.h"

int s21_get_sign(s21_decimal value);
int s21_set_bit(s21_decimal* value, int n_bits, int n);
int s21_get_exp(float f);
int s21_get_bit(s21_decimal decimal,unsigned int pos);
char s21_get_scale(s21_decimal const decimal);

#endif
