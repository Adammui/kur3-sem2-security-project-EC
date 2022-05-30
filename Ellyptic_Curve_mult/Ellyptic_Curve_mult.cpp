/*
Жулик не воруй 
--Reyn---
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "bignum.hpp"
#include <sstream>
#include <cstdio>


#define OUTPUT_SIZE 1000
#define EXTERNC extern "C"

using namespace std;

char outputX[OUTPUT_SIZE] = {};

char outputY[OUTPUT_SIZE] = {};

char* getOutputX() {
    return outputX;
}

char* getOutputY() {
    return outputY;
}

void setOutput(int x, int y) {
    memset(outputX, '\0', OUTPUT_SIZE);
    memset(outputY, '\0', OUTPUT_SIZE);

}

/* число разрядов в числах, используемых модулем,
   <= BIGNUM_MAX_DIGITS */
#define ECCRYPT_BIGNUM_DIGITS BIGNUM_MAX_DIGITS

   /* точка на эллиптической кривой */
struct eccrypt_point_t {
    bignum_digit_t x[ECCRYPT_BIGNUM_DIGITS]; /* координата x */
    bignum_digit_t y[ECCRYPT_BIGNUM_DIGITS]; /* координата y */
    int is_inf; /* является ли точка несобственной */
};

/* параметры кривой:
   коэффициенты уравнения y^2 = x^3 + a*x + b
   в поле вычетов по модулю m
   и генерирующая точка */
struct eccrypt_curve_t {
    bignum_digit_t a[ECCRYPT_BIGNUM_DIGITS];
    bignum_digit_t b[ECCRYPT_BIGNUM_DIGITS];
    bignum_digit_t m[ECCRYPT_BIGNUM_DIGITS];
    struct eccrypt_point_t g;
};

/* копирование точки */
void eccrypt_point_cpy(struct eccrypt_point_t* to, /* куда копируем */
    struct eccrypt_point_t* from) { /* откуда */
    if (to == from) return;
    if (to->is_inf = from->is_inf)
        return;

    memcpy(to->x, from->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
    memcpy(to->y, from->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
}

/* сложение точек эллиптической кривой */
void eccrypt_point_add(struct eccrypt_point_t* s, /* сумма */
    struct eccrypt_point_t* p, /* первое слогаемое */
    struct eccrypt_point_t* q, /* второе слогаемое */
    struct eccrypt_curve_t* curve)

{
    /* параметры кривой */
    struct eccrypt_point_t rslt; /* вполне возможна ситуация s = p = q */
    bignum_digit_t lambda[ECCRYPT_BIGNUM_DIGITS]; /* коэффициент лямбда */
    struct eccrypt_point_t* tp; /* временный указатель */
    int equal = 1; /* предполагаем, что точки равны */

    /* проверка на бесконечность: P + O = O + P = P для любой точки поля */
    if (p->is_inf) { tp = p; p = q; q = tp; }
    if (q->is_inf) {
        eccrypt_point_cpy(s, p);
        return;
    }

    if ((p != q) && (equal = !bignum_cmp(p->x, q->x, ECCRYPT_BIGNUM_DIGITS)))
        /* адреса точек различаются, но координаты по x равны */
        if (!(equal = !bignum_cmp(p->y, q->y, ECCRYPT_BIGNUM_DIGITS))) {
            /* проверяем случай Q = -P */
            memcpy(rslt.x, p->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS));
            bignum_madd(rslt.x, q->y, curve->m, ECCRYPT_BIGNUM_DIGITS);
            if (bignum_iszero(rslt.x, ECCRYPT_BIGNUM_DIGITS)) {
                s->is_inf = 1;
                return;
            }
        }

    /* вычисляем лямбда */
    if (equal) {
        /* lambda = (3*x*x + a) / (2*y)  (mod m) */
        bignum_setzero(rslt.x, ECCRYPT_BIGNUM_DIGITS);   /* t := 2 */
        rslt.x[0] = 2;
        bignum_mmul(rslt.x, p->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* t *= y */
        bignum_setzero(lambda, ECCRYPT_BIGNUM_DIGITS);   /* l := 3 */
        lambda[0] = 3;
        bignum_mmul(lambda, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l *= x */
        bignum_mmul(lambda, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l *= x */
        bignum_madd(lambda, curve->a, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l += a */
    }
    else {
        /* lambda = (y1 - y2) / (x1 - x2)  (mod m) */
        memcpy(rslt.x, p->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* t := x1 */
        bignum_msub(rslt.x, q->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* t -= x2 */
        memcpy(lambda, p->y, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* l := y1 */
        bignum_msub(lambda, q->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l -= y2 */
    }
    bignum_mdiv(lambda, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* l /= t */

    /* x3 = lambda*lambda - x1 - x2  (mod m) */
    memcpy(rslt.x, lambda, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* x3 := l */
    bignum_mmul(rslt.x, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 *= l */
    bignum_msub(rslt.x, p->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 -= x1 */
    bignum_msub(rslt.x, q->x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* x3 -= x2 */

    /* y3 = lambda*(x1 - x3) - y1  (mod m) */
    memcpy(rslt.y, p->x, BIGNUM_SIZE(ECCRYPT_BIGNUM_DIGITS)); /* y3 := x1 */
    bignum_msub(rslt.y, rslt.x, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 -= x3 */
    bignum_mmul(rslt.y, lambda, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 *= l */
    bignum_msub(rslt.y, p->y, curve->m, ECCRYPT_BIGNUM_DIGITS); /* y3 -= y1 */

    rslt.is_inf = 0; /* получили собственную точку */
    eccrypt_point_cpy(s, &rslt);
}

/* умножение точек эллиптической кривой */
void eccrypt_point_mul(struct eccrypt_point_t* rslt, /* результат */
    struct eccrypt_point_t* p, /* точка */
    bignum_digit_t* k, /* множитель */
    struct eccrypt_curve_t* curve)

{
    /* кривая */
    struct eccrypt_point_t point; /* копия точки */
    bignum_digit_t digit; /* значение текущего разряда множителя */
    int digit_num = 0; /* номер разряда */
    int bits = 0; /* кол-во необработанных бит в разряде */
    int n = ECCRYPT_BIGNUM_DIGITS; /* число значащих разрядов */

    if (p->is_inf) {
        rslt->is_inf = 1;
        return; /* n * O = O */
    }

    /* определяем старший значащий разряд */
    while ((n > 0) && !k[n - 1]) n--;
    /* создаем копию точки */
    if (n) eccrypt_point_cpy(&point, p);

    /* несобственная точка, раньше мы не могли менять rslt,
    так как возможна ситуация rslt == p */
    rslt->is_inf = 1;

    /* пока есть необработанные биты */
    while ((digit_num < n) || digit) {
        if (digit_num) /* это итерация не первая по счету */
            eccrypt_point_add(&point, &point, &point, curve); /* point*=2 */

        if (!bits) {
            /* текущий разряд уже обработан или еще не инициализирован */
            digit = k[digit_num++];
            bits = sizeof(bignum_digit_t) * 8;
        }

        if (digit & 1) /* rslt += point */
            eccrypt_point_add(rslt, rslt, &point, curve);

        digit >>= 1;
        bits--;
    }
}

EXTERNC char* mult(char* a, char* b, char* p, char* pointX, char* pointY, char* multk) {
//int main(int argc, char** argv) {

   /* char a[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
    char b[] = "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b";
    char p[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF";
     char pointX[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
    char pointY[] = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
    char multk[] = "70B99F5ADCFDBEF81058967018BA95736AA2B60060EC25FFFFFFFFFFFFFFFFFF";
*/
    //точка генерирующая кривую(базовая точка)
    //char gx[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296"; 
    //char gy[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";
   

    struct eccrypt_curve_t curve;
    struct eccrypt_point_t p1, rslt;
    bignum_digit_t k[ECCRYPT_BIGNUM_DIGITS];
    char buff[256], buff1[256];
    char* newX; char* newY;
 
    /* инициализируем параметры кривой */
    bignum_fromhex(curve.a, a, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(curve.b, b, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(curve.m, p, ECCRYPT_BIGNUM_DIGITS);
    //bignum_fromhex(curve.g.x, gx, ECCRYPT_BIGNUM_DIGITS);
    //bignum_fromhex(curve.g.y, gy, ECCRYPT_BIGNUM_DIGITS);
    curve.g.is_inf = 0;
    // создаем точку на этой кривой из параметров п
    bignum_fromhex(p1.x, pointX, ECCRYPT_BIGNUM_DIGITS);
    bignum_fromhex(p1.y, pointY, ECCRYPT_BIGNUM_DIGITS);
    // проверка несобственности точки
    p1.is_inf = bignum_iszero(p1.x, ECCRYPT_BIGNUM_DIGITS) &&
        bignum_iszero(p1.y, ECCRYPT_BIGNUM_DIGITS);
    //умножение точки p1 на скаляр K, записывает в перементтую ресалт
    bignum_fromhex(k, multk, ECCRYPT_BIGNUM_DIGITS);
    
    //начало таймера
    clock_t start = clock();
    //
    eccrypt_point_mul(&rslt, &p1, k, &curve);
    // конец таймера
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    char time[100];
    sprintf(time, " % f", seconds);

    //для вывода полученной умноженной точки
    if (rslt.is_inf) {
        printf("0\n0\n(infinite point)\n");
    }
    else {
        bignum_tohex(rslt.x, buff, sizeof(buff), ECCRYPT_BIGNUM_DIGITS);
        std::cout << buff << std::endl;
        newX = buff;
        bignum_tohex(rslt.y, buff1, sizeof(buff1), ECCRYPT_BIGNUM_DIGITS);
        std::cout << buff1 << std::endl;
        newY = buff1;
        std::cout << time << std::endl;
    }
    
    char result[10000] = {};
    sprintf(result, "{\"x\":\"%s\", \"y\":\"%s\", \"time\":\"%s\"}", newX,newY,time);
    return result;
   // return 1;
   // return newX;// , newY, time;
}
int main() {
    return 0;
}
