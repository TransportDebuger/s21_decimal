#include <math.h>

#include "common.h"

int s21_negate(s21_decimal value, s21_decimal* result) {
  *result = value;
  s21_set_sign(result, !s21_get_sign(value));
  return 0;
}
int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int error = 1;
  dst->bits[LOW] = 0;
  dst->bits[MID] = 0;
  dst->bits[HIGH] = 0;
  dst->bits[SCALE] = 0;
  if (dst != NULL) {
    if (src < 0) {
      s21_set_sign(dst, 1);
      src = -src;
    }
    dst->bits[LOW] = abs(src);
    error = 0;
  }

  return error;  // 4234782423
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int rez = 0;
  if (result) {
    int sign = s21_get_sign(value);
    int scale = s21_get_scale(value);
    *result = divideBinaryByTen(value, scale);
    s21_set_scale(result, 0);
    s21_set_sign(result, sign);
  } else {
    rez = 1;
  }
  return rez;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int rez = 1;
  int error = 0;
  dst->bits[LOW] = 0;
  dst->bits[MID] = 0;
  dst->bits[HIGH] = 0;
  dst->bits[SCALE] = 0;
  if (src && dst) {  // отдалить
    int sign = get_float_sign(src);
    int exp = s21_get_exp(src);
    if (isinf(src) || isnan(src)) error = 1;
    if (!error && src != 0) {
      double temp = (double)fabs(src);
      int scale = 0;
      while (scale < 28 && (int)temp / (int)pow(2, 21) ==
                               0) {  // чем выше степень двойки тем точнее число
                                     // именьше погрешность
        temp *= 10;
        scale++;
      }
      temp = round(temp);
      if (scale <= 28 && (exp > -94 && exp < 96)) {
        floatbits mantissa;

        while (fmod(temp, 10) == 0 && scale > 0) {
          scale--;
          temp /= 10;
        }
        mantissa.fl = temp;
        exp = s21_get_exp(mantissa.fl);
        dst->bits[exp / 32] |= 1 << exp % 32;
        for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
          if ((mantissa.ui & (1 << j)) != 0) {
            dst->bits[i / 32] |= 1 << i % 32;
          }
        }
        s21_set_scale(dst, scale);
        s21_set_sign(dst, sign);
        rez = 0;
      }
    }
  }
  return rez;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int rez = 1;
  if (dst) {
    double temp = 0;
    int scale = 0;
    for (int i = 0; i < 96; i++)
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) temp += pow(2, i);
    if ((scale = s21_get_scale(src)) > 0) {
      for (int i = scale; i > 0; i--) temp /= 10.0;
    }
    *dst = (float)temp;
    *dst *= s21_get_sign(src) ? -1 : 1;
    rez = 0;
  }
  return rez;
}
int s21_floor(s21_decimal value, s21_decimal* result) {
  int rez = 0;
  if (result) {
    int sign = s21_get_sign(value);
    s21_decimal zero = {{0, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal temp = {{0, 0, 0, 0}};
    s21_truncate(value, result);
    s21_truncate(value, &temp);
    if (s21_is_less(value, zero) && s21_is_not_equal(value, temp))
      s21_sub(*result, one, result);
    s21_set_sign(result, sign);
  } else {
    rez = 1;
  }
  return rez;
}
int s21_round(s21_decimal value, s21_decimal* result) {
  int rez = 0;
  int flag = 0;
  if (result) {
    s21_decimal zero = {{0, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal temp = {{0, 0, 0, 0}};
    s21_decimal half = {{0, 0, 0, 0}};
    s21_decimal mantissa = {{0, 0, 0, 0}};
    s21_from_float_to_decimal(0.5, &half);
    if (s21_is_less(value, zero)) {
      s21_negate(value, &value);
      flag = 1;
    }
    s21_truncate(value, &temp);
    s21_sub(value, temp, &mantissa);

    float mant_fl, half_fl;
    s21_from_decimal_to_float(mantissa, &mant_fl);
    s21_from_decimal_to_float(half, &half_fl);
    if (mant_fl >= half_fl)
      s21_add(temp, one, result);
    else
      *result = temp;

    if (flag) s21_negate(*result, result);
  } else {
    rez = 1;
  }
  return rez;
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;
  int sign_1 = s21_get_sign(dec1);
  int sign_2 = s21_get_sign(dec2);

  if (sign_1 == sign_2) {
    int exp1 = s21_get_scale(dec1), exp2 = s21_get_scale(dec2);

    if (exp1 > exp2) {
      dec2 = multiplyByTen(dec2, exp1 - exp2);
      s21_set_sign(&dec2, sign_2);
    } else if (exp1 < exp2) {
      dec1 = multiplyByTen(dec1, exp2 - exp1);
      s21_set_sign(&dec1, sign_1);
    }

    if (!(dec1.bits[HIGH] ^ dec2.bits[HIGH]) &&
        !(dec1.bits[MID] ^ dec2.bits[MID]) &&
        !(dec1.bits[LOW] ^ dec2.bits[LOW])) {
      rez = 1;
    }
  }

  return rez;
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;
  int sign_1 = s21_get_sign(dec1);
  int sign_2 = s21_get_sign(dec2);
  if (sign_1 > sign_2) {
    rez = 1;
  } else if (sign_1 == sign_2) {
    int exp1 = s21_get_scale(dec1), exp2 = s21_get_scale(dec2);
    if (exp1 > exp2) {
      dec2 = multiplyByTen(dec2, exp1 - exp2);
      s21_set_sign(&dec2, sign_2);
    } else if (exp1 < exp2) {
      dec1 = multiplyByTen(dec1, exp2 - exp1);
      s21_set_sign(&dec1, sign_1);
    }

    int bit_1 = s21_get_high_bit(dec1);
    int bit_2 = s21_get_high_bit(dec2);
    if (bit_1 != bit_2) {
      if (!sign_1 && !sign_2) {
        if (bit_1 < bit_2) {
          rez = 1;
        }
      } else {
        if (bit_1 > bit_2) {
          rez = 1;
        }
      }
    }
    for (int i = (bit_1 + bit_2) / 2; i >= 0 && bit_1 == bit_2; i--) {
      bit_1 = s21_get_bit(dec1, i);
      bit_2 = s21_get_bit(dec2, i);
      if (bit_1 < bit_2 && !(sign_1) && !(sign_2)) {
        rez = 1;
        break;
      } else if (bit_1 > bit_2 && sign_1 && sign_2) {
        rez = 1;
        break;
      }
    }
  }
  return rez;
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;

  if (s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2)) {
    rez = 1;
  }
  return rez;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;

  if (!s21_is_less(dec1, dec2) && s21_is_not_equal(dec1, dec2)) {
    rez = 1;
  }

  return rez;
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;

  if (!s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2)) {
    rez = 1;
  }

  return rez;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  int rez = 0;

  if (!(s21_is_equal(dec1, dec2))) {
    rez = 1;
  }

  return rez;
}
int sum(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int bit_shift = 0;
  int rez = 0;
  for (int i = 0; i <= 95; i++) {
    int bit_value1 = s21_get_bit(value_1, i);
    int bit_value2 = s21_get_bit(value_2, i);
    int temp = bit_value1 + bit_value2 + bit_shift;
    if (temp == 0) {
      s21_set_bit(result, i, 0);
      bit_shift = 0;
    }
    if (temp == 1) {
      s21_set_bit(result, i, 1);
      bit_shift = 0;
    }
    if (temp == 2 || temp == 3) {
      s21_set_bit(result, i, temp == 2 ? 0 : 1);
      bit_shift = 1;
    }
  }
  return rez;
}
int get_inverse(s21_decimal* value) {
  int rez = 0;
  value->bits[LOW] = ~value->bits[LOW];
  value->bits[MID] = ~value->bits[MID];
  value->bits[HIGH] = ~value->bits[HIGH];
  s21_decimal one = {{1, 0, 0, 0}};
  rez = sum(*value, one, value);
  // printf("after sum\n");
  // print_decimal(*value);
  //  printf("\n");
  return rez;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int rez = 0;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int exp1 = s21_get_scale(value_1);
  int exp2 = s21_get_scale(value_2);
  s21_decimal MAX = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal MIN = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, ~(UINT32_MAX / 2)}};
  s21_decimal zero = {{0, 0, 0, 0}};

  if (exp1 < exp2) {
    value_1 = multiplyByTen(value_1, exp2 - exp1);
    s21_set_scale(result, exp2);
    s21_set_scale(&value_1, exp2);
  } else if (exp1 > exp2) {
    value_2 = multiplyByTen(value_2, exp1 - exp2);
    s21_set_scale(result, exp1);
    s21_set_scale(&value_2, exp1);
  } else {
    s21_set_scale(result, (exp1 + exp2) / 2);
  }
  if ((s21_is_equal(MAX, value_1) || s21_is_equal(MAX, value_2)) &&
      (s21_is_greater(value_1, zero) && s21_is_greater(value_2, zero))) {
    return 1;
  }
  if ((s21_is_equal(MIN, value_1) || s21_is_equal(MIN, value_2)) &&
      (s21_is_less(value_1, zero) && s21_is_less(value_2, zero))) {
    return 2;
  }
  if (sign1 == sign2) {
    rez = sum(value_1, value_2, result);
  } else if (sign1 > sign2) {
    int flag = s21_get_bit(value_1, 95);
    get_inverse(&value_1);
    rez = sum(value_1, value_2, result);
    if (s21_get_bit(*result, 95) == 1 || flag) {
      get_inverse(result);
      s21_set_sign(result, 1);
    }
  } else if (sign1 < sign2) {
    int flag = s21_get_bit(value_2, 95);
    get_inverse(&value_2);
    rez = sum(value_1, value_2, result);
    if (s21_get_bit(*result, 95) == 1 || flag) {
      get_inverse(result);
      s21_set_sign(result, 1);
    }
  }
  if (sign1 && sign2) s21_negate(*result, result);
  return rez;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int rez = 0;
  s21_decimal zero = {{0, 0, 0, 0}};
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  if (sign1 != sign2) {
    if (sign1 == 1 && sign2 == 0) {
      s21_set_sign(&value_2, 1);
      rez = s21_add(value_1, value_2, result);
    } else {
      s21_set_sign(&value_2, 0);
      rez = s21_add(value_1, value_2, result);
    }
  } else if (sign1 == 1 && sign2 == 1) {
    if (s21_is_less(value_1, value_2)) {
      s21_set_sign(&value_2, 0);
      rez = s21_add(value_2, value_1, result);
    } else {
      s21_set_sign(&value_2, 0);

      rez = s21_add(value_1, value_2, result);
    }
  } else if ((sign1 == 0 && sign2 == 0) && s21_is_not_equal(value_2, zero)) {
    s21_set_sign(&value_2, 1);
    rez = s21_add(value_2, value_1, result);
  } else {
    rez = s21_add(value_1, value_2, result);
  }

  return rez;
}

int mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int rez;
  int bit_shift = 0;
  for (int i = 0; i <= 95; i++) {
    int bit_value_1 = s21_get_bit(value_1, i);
    int bit_value_2 = s21_get_bit(value_2, i);
    if (bit_value_1 && bit_value_2) {
      if (bit_shift) {
        s21_set_bit(result, i, 1);
      } else {
        s21_set_bit(result, i, 0);
        bit_shift = 1;
      }
    } else if (!bit_value_1 && !bit_value_2) {
      if (bit_shift) {
        s21_set_bit(result, i, 1);
        bit_shift = 0;
      } else {
        s21_set_bit(result, i, 0);
      }
    } else if (bit_value_1 != bit_value_2) {
      if (bit_shift) {
        s21_set_bit(result, i, 0);
      } else {
        s21_set_bit(result, i, 1);
      }
    }
    if (i == 95 && bit_shift == 1) rez = 1;
  }
  return rez;
}
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int rez = 0;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int exp1 = s21_get_scale(value_1);
  int exp2 = s21_get_scale(value_2);
  int sign_rez;
  int scale_rez = exp1 + exp2;
  if (sign1 != sign2)
    sign_rez = 1;
  else
    sign_rez = 0;

  for (int i = 0; i <= s21_get_high_bit(value_1); i++) {
    s21_decimal temp = {0};
    int bit_value_1 = s21_get_bit(value_1, i);
    if (bit_value_1) {
      if (95 - s21_get_high_bit(value_1) - 1 == i && sign_rez) {
        rez = 1;
        break;
      } else if (95 - s21_get_high_bit(value_1) - 1 == i && !sign_rez) {
        rez = 2;
        break;
      }
      temp = value_2;
      s21_shift_left(&temp, i);
      rez = mul(*result, temp, result);
    }
  }
  s21_decimal t = *result;
  while (scale_rez > 28) {
    s21_set_scale(&t, 1);
    s21_truncate(t, result);
    s21_decimal fraction = {{0, 0, 0, 0}};
    s21_decimal five = {{5, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_sub(t, *result, &fraction);
    s21_set_scale(&fraction, 0);
    float a;
    s21_from_decimal_to_float(fraction, &a);
    if (s21_is_greater(fraction, five)) {
      s21_add(*result, one, result);
    } else {
      if (s21_get_bit(*result, 0) && s21_is_equal(fraction, five))
        s21_add(*result, one, result);
    }
    scale_rez--;
    t = *result;
  }

  s21_set_scale(result, scale_rez);
  s21_set_sign(result, sign_rez);
  return rez;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int rez = 1;
  s21_truncate(src, &src);
  if (dst != NULL && src.bits[MID] == 0 && src.bits[HIGH] == 0) {
    *dst = src.bits[0];
    *dst *= s21_get_sign(src) ? -1 : 1;
    rez = 0;
  }
  return rez;
}

int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result) {  //версия 5.0
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 0;
  int common_sign = (!(s21_get_sign(value_1) == s21_get_sign(value_2)));
  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);
  s21_decimal null = {{0, 0, 0, 0}};
  s21_decimal tmp = {{0, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  s21_set_sign(&value_1, 0);
  s21_set_sign(&value_2, 0);
  if (s21_is_equal(value_2, null)) {  //обрабатываем деление на 0 (валуй 2 == 0)
    *result = null;
    res = 3;
  } else {
    if (s21_is_equal(value_1,
                     value_2)) {  //обрабатваем деление равных по модулю чисел
                                  //(абс (валуя 1) == абс (валуя 2))
      *result = one;
      s21_set_sign(result, common_sign);
    }
    if (s21_is_less(value_2,
                    value_1)) {  //обрабатываем случай деления: делимое по
                                 //модулю больше делителя (валуй 1 > валуя 2)
      div_scale_equalizer(&value_1, &value_2, scale1, scale2);
      div_basic(value_1, value_2, result, &tmp);
      s21_mul(*result, value_2, &check);
      int j = 0;
      while ((s21_is_less(check, value_1)) && (j <= 8)) {
        value_1 = multiplyByTen(value_1, 1);
        j++;
        div_basic(value_1, value_2, result, &tmp);
        check = null;
        s21_mul(*result, value_2, &check);
      }
      s21_set_scale(result, j);
      s21_set_sign(result, common_sign);
    }
    if (s21_is_less(value_1,
                    value_2)) {  // обрабатываем случай деления: делимое по
      // модулю меньше делителя (валуй 1 < валуя 2)
      int i = 0;
      div_scale_equalizer(&value_1, &value_2, scale1, scale2);
      while (s21_is_less(value_1, value_2) && (i <= 8)) {
        i++;
        value_1 = multiplyByTen(value_1, 1);
      }
      div_basic(value_1, value_2, result, &tmp);
      s21_mul(*result, value_2, &check);
      while ((s21_is_less(check, value_1)) && (i <= 8)) {
        value_1 = multiplyByTen(value_1, 1);
        i++;
        div_basic(value_1, value_2, result, &tmp);
        check = null;
        s21_mul(*result, value_2, &check);
      }
      s21_set_scale(result, i);
      s21_set_sign(result, common_sign);
    }
  }

  return res;
}

void div_scale_equalizer(s21_decimal* value_1, s21_decimal* value_2, int scale1,
                         int scale2) {
  if (scale1 == scale2) {
    s21_set_scale(value_1, 0);
    s21_set_scale(value_2, 0);
  } else {
    if (scale1 > scale2) {  // 0,8:4
      *value_2 = multiplyByTen(*value_2, scale1 - scale2);
      s21_set_scale(value_1, 0);
      s21_set_scale(value_2, 0);
    }
    if (scale1 < scale2) {  // 10/0.07 = 100/0.7 = 1000/7
      *value_1 = multiplyByTen(*value_1, scale2 - scale1);
      s21_set_scale(value_1, 0);
      s21_set_scale(value_2, 0);
    }
  }
}

void div_basic(s21_decimal value_1, s21_decimal value_2, s21_decimal* result,
               s21_decimal* tmp) {
  tmp->bits[0] = tmp->bits[1] = tmp->bits[2] = tmp->bits[3] = 0;
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  for (int i = s21_get_high_bit(value_1); i >= 0; i--) {
    if (s21_get_bit(value_1, i)) s21_set_bit(tmp, 0, 1);
    if (s21_is_greater_or_equal(*tmp, value_2)) {
      s21_sub(*tmp, value_2, tmp);
      if (i != 0) s21_shift_left(tmp, 1);
      if (s21_get_bit(value_1, i - 1)) s21_set_bit(tmp, 0, 1);
      s21_shift_left(result, 1);
      s21_set_bit(result, 0, 1);
    } else {
      s21_shift_left(result, 1);
      if (i != 0) s21_shift_left(tmp, 1);
      if ((i - 1) >= 0 && s21_get_bit(value_1, i - 1)) s21_set_bit(tmp, 0, 1);
    }
  }
}
