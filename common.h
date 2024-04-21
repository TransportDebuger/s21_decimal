#include "s21_decimal.h"

int s21_get_sign(s21_decimal value);
int s21_set_sign(s21_decimal* value, int sign);
int s21_set_bit(s21_decimal* value, int n_bits, int n);
int s21_get_exp(float f);
int s21_get_bit(s21_decimal decimal, unsigned int pos);
int s21_get_scale(s21_decimal const decimal);
int s21_get_high_bit(s21_decimal value);
int s21_shift_left(s21_decimal* value, int num);
int s21_set_scale(s21_decimal* number, int scale);
// char* divideBinaryByTen(const char* binaryNumber, int scale);
// char* multiplyByTen(const char* binaryNumber, int scale);
// char* decimal_to_char(s21_decimal src);
// s21_decimal char_to_decimal(char* value);
int get_float_sign(float num);

s21_decimal divideBinaryByTen(s21_decimal value, int scale);
s21_decimal multiplyByTen(s21_decimal value, int scale);
