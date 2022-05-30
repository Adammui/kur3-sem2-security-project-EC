
/* ����������� �����, ������������ ��� �������� ������ ������� �����
��� 32-� ��������� ����� - unsiged short, ��� 64-� ��������� - unsigned int */
typedef unsigned short bignum_digit_t;

/* ����������� �����, ������� � ��� ���� ������ ���, ��� bignum_digit_t
��� 32-� ��������� ����� - unsiged int, ��� 64-� ��������� - unsigned long */
typedef unsigned int bignum_double_t;

/* ����������� �����, ������� � ��� ���� ������ ���, ��� bignum_digit_t
��� 32-� ��������� ����� - siged int, ��� 64-� ��������� - signed long */
typedef signed int bignum_signed_double_t;

/* ���-�� ��� � ����� ������� ����� */
#define BIGNUM_DIGIT_BITS (sizeof(bignum_digit_t) * 8)

/* ����� ��� ��������� �������� ����� */
#define BIGNUM_DIGIT_MASK (((bignum_double_t)1 << BIGNUM_DIGIT_BITS) - 1)

/* ������, ����������� ���-�� �������� � ����� �� ��� ������� */
#define BIGNUM_DIGITS(x) (( x ) / sizeof(bignum_digit_t) )

/* ������, ����������� ������ ����� �� ���-�� �������� � ��� */
#define BIGNUM_SIZE(x) (( x ) * sizeof(bignum_digit_t) )

/* ������������ ������ ����� � ������ */
#define BIGNUM_MAX_SIZE 32

/* ����������� ���������� ���-�� �������� � ����� */
#define BIGNUM_MAX_DIGITS  (BIGNUM_MAX_SIZE / sizeof(bignum_digit_t))

/* �������������� ����� � ������ */
void bignum_tohex(bignum_digit_t* num, char* buff, int buffsize, int digits);

/* �������������� ������ � ����� */
void bignum_fromhex(bignum_digit_t* num, char* str, int digits);

/* �������� �� ����� ����� */
int bignum_iszero(bignum_digit_t* num, int digits);

/* ��������� ����� �������� ���� */
void bignum_setzero(bignum_digit_t* num, int digits);

/* ��������� ���� �����: 0 => �����, 1 => a > b, -1 => a < b*/
int bignum_cmp(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* ����������� ����� */
void bignum_cpy(bignum_digit_t* to, bignum_digit_t* from,
                int to_digits, int from_digits);

/* ��������� b � a. digits - ���-�� �������� � ������ */
void bignum_add(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* ������� b �� a. digits - ���-�� �������� � ������ */
void bignum_sub(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* �������� a �� b */
void bignum_mul(bignum_digit_t* a, bignum_digit_t* b, int digits);

/* ��������� ������� a �� b, ����� ������� � (�����������) �������.
��������� �������� ��������� ��������� � �����, ������� � ����������� */
void bignum_div(bignum_digit_t* a, /* ������� */
                bignum_digit_t* b, /* �������� */
                bignum_digit_t* q, /* �������, ����� ���� a, b ��� NULL */
                bignum_digit_t* r, /* �������, ����� ���� a, b ��� NULL */
                int digits); /* ���-�� �������� � ������ */

/* ����� ����� a � b � ���� ������� �� ������ m
������������ ������� ������ ������ */
void bignum_madd(bignum_digit_t* a, bignum_digit_t* b,
                 bignum_digit_t* m, int digits);

/* ����� �������� a � b � ���� ������� �� ������ m
������������ ������� ������ ������ */
void bignum_msub(bignum_digit_t* a, bignum_digit_t* b,
                 bignum_digit_t* m, int digits);

/* ����� ������������ a �� b � ���� ������� �� ������ m
������������ ������� ������ ������ */
void bignum_mmul(bignum_digit_t* a, bignum_digit_t* b,
                 bignum_digit_t* m, int digits);

/* ����� �������, �������� num � ���� ������� �� ������ m */
void bignum_inv(bignum_digit_t* num, bignum_digit_t* m, int digits);

/* ����� ������� a � b � ���� ������� �� ������ m
������������ ������� ������ ������ */
void bignum_mdiv(bignum_digit_t* a, /* ������� */
                 bignum_digit_t* b, /* �������� */
                 bignum_digit_t* m, /* ������ */
                 int digits); /* ���-�� �������� � ������ */