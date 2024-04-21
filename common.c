#include "common.h"

int s21_get_sign(s21_decimal value) {
  int sign = 1 << 31;
  return (value.bits[SCALE] & sign) ? 1 : 0;
}
int s21_set_sign(s21_decimal *value, int sign) {
  return s21_set_bit(value, 127, sign);
}
int s21_set_bit(s21_decimal *value, int num_bits, int value_bit) {
  unsigned int mask = 1u << (num_bits % 32);
  if (num_bits <= 127) {
    if (num_bits / 32 < 4 && value_bit) {
      value->bits[num_bits / 32] |= mask;
    } else if (num_bits / 32 < 4 && !value_bit) {
      value->bits[num_bits / 32] &= ~mask;  // ~ это побитовая инверсия
      // мы сбрасываем то значение которое было здесь. теперь оно = 0
    }
  }
  return 0;
}

int s21_get_exp(float num) {
  uint32_t *ptr = (uint32_t *)&num;
  uint32_t numAsInt = *ptr;
  return (((numAsInt >> 23) & 0xFF) - 0x7F);
}
int get_float_sign(float num) {
  uint32_t *ptr = (uint32_t *)&num;
  uint32_t numAsInt = *ptr;
  return ((numAsInt >> 31) & 1);
}
int s21_get_bit(s21_decimal decimal, unsigned int pos) {
  uint32_t rez = 0;
  uint32_t mask = (1 << pos);

  if (pos < 32) rez = decimal.bits[LOW] & mask;
  if (pos >= 32 && pos <= 63) rez = decimal.bits[MID] & mask;
  if (pos >= 64 && pos <= 95) rez = decimal.bits[HIGH] & mask;
  if (pos >= 96 && pos <= 127) rez = decimal.bits[SCALE] & mask;

  return (rez == 0) ? 0 : 1;
}

int s21_get_scale(s21_decimal const decimal) {
  union union_scale us = {0};
  us.int_union_scale = decimal.bits[SCALE];

  return (int)us.parts[2];
}
int s21_get_high_bit(s21_decimal value) {
  int count = 95;
  for (; count >= 0 && s21_get_bit(value, count) == 0; count--) {
  }
  return count;
}
int s21_shift_left(s21_decimal *value, int num) {
  int rez = 0;
  int lastbit = s21_get_high_bit(*value);
  if (lastbit + num > 95) {
    rez = 1;
  }
  if (!rez) {
    for (int i = 0; i < num; i++) {
      int bit_31 = s21_get_bit(*value, 31);
      int bit_63 = s21_get_bit(*value, 63);
      value->bits[LOW] <<= 1;
      value->bits[MID] <<= 1;
      value->bits[HIGH] <<= 1;
      if (bit_31) s21_set_bit(value, 32, 1);
      if (bit_63) s21_set_bit(value, 64, 1);
    }
  }
  return rez;
}
int s21_set_scale(s21_decimal *number, int scale) {
  union union_scale us = {.int_union_scale = number->bits[SCALE]};
  us.parts[2] = (char)scale;
  number->bits[SCALE] = us.int_union_scale;
  return 0;
}
s21_decimal divideBinaryByTen(s21_decimal value, int scale) {
  s21_decimal rez = {0};
  rez = value;
  for (int j = 0; j < scale; j++) {
    int carry = 0;  // перенос

    for (int i = 95; i >= 0; i--) {
      int digit = s21_get_bit(value, i) + carry * 2;
      int temp = digit / 10;  // делим текущую цифру на 10
      s21_set_bit(&rez, i, temp);
      carry = digit % 10;  // сохраняем остаток для следующей итерации
    }
    value = rez;
  }

  return rez;
}

s21_decimal multiplyByTen(s21_decimal value, int scale) {
  s21_decimal rez = {0};
  rez = value;
  for (int j = 0; j < scale; j++) {
    int carry = 0;  // перенос

    for (int i = 0; i <= 95; i++) {
      int digit = s21_get_bit(value, i);
      int product = digit * 10 + carry;
      int temp = product % 2;  // делим текущую цифру на 10
      s21_set_bit(&rez, i, temp);
      carry = product / 2;  // сохраняем остаток для следующей итерации
    }
    value = rez;
  }

  return rez;
}
