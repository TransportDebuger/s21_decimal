#include "./s21_decimal.h"

int s21_get_sign(s21_decimal value) {
  int sign = 1 << 31;
  return (value.bits[SERVICE] & sign) ? 1 : 0;
}

int s21_get_bit(s21_decimal decimal, unsigned int pos) {
  uint32_t rez = 0;
  uint32_t mask = (1 << pos);

  if (pos < 32) rez = decimal.bits[LOW] & mask;
  if (pos >= 32 && pos <= 63) rez = decimal.bits[MID] & mask;
  if (pos >= 64 && pos <= 95) rez = decimal.bits[HIGH] & mask;
  if (pos >= 96 && pos <= 127) rez = decimal.bits[SERVICE] & mask;

  return (rez == 0) ? 0 : 1;
}

int s21_set_bit(s21_decimal *value, int n_bits, int n) {
  // value->bits[n_bits] ^= 1U << n;
  unsigned int mask = 1u << (n_bits % 32);
  if (n_bits / 32 < 4 && n) {
    value->bits[n_bits / 32] |= mask;
  } else if (n_bits / 32 < 4 && !n) {
    value->bits[n_bits / 32] &= ~mask;  // ~ это побитовая инверсия
    // мы сбрасываем то значение которое было здесь. теперь оно = 0
  }

  return 0;
}

int s21_get_offset(s21_decimal const decimal) {
  union union_scale us = {0};
  us.int_union_scale = decimal.bits[SCALE];

  return (int)us.parts[2];
}