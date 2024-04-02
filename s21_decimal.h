#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define MAX_OFFSET 28 //максимальное смещение запятой (порядок)
#define MIN_OFFSET 0  //минимальное смещение запятой (порядок)
#define SIGN_BIT 31

enum byte {
    HIGH = 0,
    MID,
    LOW,
    SERVICE
};

typedef struct {
    int bits[4];
} s21_decimal;

int s21_get_sign(s21_decimal value);
int s21_set_bit(s21_decimal* value, int n_bits, int n);
int s21_get_exp(float f);
int s21_get_bit(s21_decimal decimal,unsigned int pos);
char s21_get_scale(s21_decimal const decimal);

/*тип s21_decimal
для реализации: 
bits[0]:
    - биты 0-15 принудительно занулены
    - биты 16-23 показатель степени (порядок) 10^28
    - биты 24-30 не используются, принудительно зануляются.
    - бит 31 или бит знака (значение 0 - положительный, 1 - отрицательный)
bits[1-3]: под целочисленное представление числа.
*/

//int sign = bits[0] >> 31
//int offset = (bits[0] << 1) >> 16

//Изменение знака переменной
int s21_negate(s21_decimal value, s21_decimal *result);

//Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

//Сравнение

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

/*| Преобразователь | Функция | 
| ------ | ------ |
| Из int | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| Из float  | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| В int  | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| В float  | int s21_from_decimal_to_float(s21_decimal src, float *dst) |
*/
/* 
- Приведение к типу.
Конвертация float должна осуществлять за счет умножения на 10 в степени, обеспечивающей его приведение к целому числу.
Если степень превышает 28, то целое значение берется как число, а оставшаяся дробная часть добавляет 1 или оставляет его неизменным исходя из банковского округления.
в порядок заносится число, показывающее расположение плавающей запятой.

Целое число должно записываться так ка есть, при этом в порядок заносится 0.

- Сравнение.
Проще всего осуществлять побитово. но следует обратить внимание на числа записанные разными способами 10*10^1 или 100^10^0. Соответственно необходимо строгое приведение чисел.

- Арифметические операции:
    Сложение. Его реализация позволит решить задачу вычитания, умножения, деления.
    Вычитание, тоже сложение при изменении знака у второго числа.
    Умножение. Рекурсия, до достижения результата.
    Деление. Вычитание до достижения результата.

план решения:
1. Реализация типа.
2. Функции конвертации типов.
3. Функция изменения знака
4. Функции округления, выделения целой части
5. Арифметические функции.

*/



#endif