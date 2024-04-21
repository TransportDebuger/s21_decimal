#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union union_scale {
  char parts[4];
  int int_union_scale;
};
typedef union {
  int ui;
  float fl;
} floatbits;

typedef union {
  int ui;
  double fl;
} doublebits;

typedef struct {
  int bits[4];
} s21_decimal;

enum s21_decimal_nbyte {
  LOW,
  MID,
  HIGH,
  SCALE,
};

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);

int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);

int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_less(s21_decimal dec1, s21_decimal dec2);
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int get_inverse(s21_decimal* value);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

void div_basic(s21_decimal value_1, s21_decimal value_2, s21_decimal* result,
               s21_decimal* tmp);
void div_scale_equalizer(s21_decimal* value_1, s21_decimal* value_2, int scale1,
                         int scale2);
