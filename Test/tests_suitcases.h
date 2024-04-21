#ifndef SRC_TESTS_TESTS_SUITCASES_H_
#define SRC_TESTS_TESTS_SUITCASES_H_
// #include "../s21_decimal.h"
#include "../common.h"
#define RESET "\033[0m"
#define RED "\e[0;91m"
#define GREEN "\033[32m"
#define BLUE "\e[0;94m"

#define S21_TRUE 1
#define S21_FALSE 0
#define CONVERTERS_S21_TRUE 0
#define CONVERTERS_S21_FALSE 1
#define U_MAX_INT 4294967295          // 0b11111111111111111111111111111111
#define MAX_INT 2147483647            // 0b01111111111111111111111111111111
#define EXPONENT_MINUS_1 2147549184   // 0b10000000000000010000000000000000
#define EXPONENT_PLUS_1 65536         // 0b00000000000000010000000000000000
#define EXPONENT_PLUS_2 196608        // 0b00000000000000110000000000000000
#define EXPONENT_MINUS_28 2149318656  // 0b10000000000111000000000000000000
#define EXPONENT_PLUS_28 1835008      // 0b00000000000111000000000000000000
#define MINUS 2147483648              // 0b10000000000000000000000000000000
#define MAX_DECIMAL 79228162514264337593543950335.0F
#define MIN_DECIMAL -79228162514264337593543950335.0F
#define TRUE 1
#define FALSE 0

#define S21_OK 0
#define S21_TOO_LARGE_OR_INFINITY 1
#define S21_TOO_SMALL_OR_NEGATIVE_INFINITY 2
#define S21_DIVISION_BY_ZERO 3

#define S21_NULL_POINTER_ERROR 1

// Размеры структур
#define S21_DECIMAL_SIZE 4
#define S21_BIG_DECIMAL_SIZE 8

// Количество битов в unsigned int
#define S21_BITS_IN_UNSIGNED_INT (sizeof(unsigned int) * 8)

#define S21_MAX_UINT 4294967295

#define MAX_FLOAT 79228157791897854723898736640.0f
#define MIN_FLOAT \
  0.00000000000000000000000000010000000031710768509710513471352647538147514756461109f

#include <check.h>
#include <limits.h>
#include <stdbool.h>

// #include "../common.h"

Suite *suite_add(void);
Suite *suite_sub(void);
Suite *suite_div(void);
Suite *suite_mod(void);
Suite *suite_mul(void);
Suite *suite_from_decimal_to_float(void);
Suite *suite_from_decimal_to_int(void);
Suite *suite_from_float_to_decimal(void);
Suite *suite_from_int_to_decimal(void);
Suite *suite_is_equal(void);
Suite *suite_is_not_equal(void);
Suite *suite_is_greater(void);
Suite *suite_is_greater_or_equal(void);
Suite *suite_is_less(void);
Suite *suite_is_less_or_equal(void);
Suite *suite_truncate(void);
Suite *suite_negate(void);
Suite *suite_floor(void);
Suite *suite_round(void);
void run_tests(void);
void run_testcase(Suite *testcase);

#endif  // SRC_TESTS_TESTS_SUITCASES_H_
