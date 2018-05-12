#pragma once
#include <iostream>
#include <cstring>
using namespace std;

typedef int BigError;
typedef unsigned int base;
typedef unsigned long long doubleBase;

namespace BigErrors {
static const BigError OK = 0;
static const BigError INCORRECT_SYMBOL = 1;
static const BigError INCOMPATIBLE_OPERANDS = 2;
static const BigError DIV_ZERO = 3;
};  // namespace BigErrors

class Big {
public:
    base *al;  //в начале всегда
    base *ar;
    base *ah;  // range of allocation memory

    Big();
    ~Big();
    Big(const Big &);

    int Rand(int bound);
    int GetCapacity() const;
    int GetLength() const;
    void Resize(int);  //количество блоков
    void Compress();
    Big Mul(base);
    Big Div(base, base &);
    friend int Compare(const Big &b, const Big &a);
    friend int CompareWithZero(const Big &b);
    friend int CompareWithConst(const Big &b, base a);
    friend Big Substraction(Big &b, Big &a, int &flag);
    friend Big Division(Big &, Big &, Big &);
    friend Big Degree(Big &x, Big &y, Big &mod);
    friend Big GetZForBurretReduction(Big &mod);
    friend Big BurretReduction(Big &x, Big &mod, Big &z);
    friend Big Karatsuba(Big &u, Big &v);
    friend bool MillerRabin(Big &n, int t);

    Big &operator=(const Big &a);  // this = a
    Big &operator=(base a);
    Big &operator=(doubleBase a);
    friend Big operator+(Big &a, Big &b);
    friend Big operator-(Big &a, Big &b);  // b-a
    friend Big operator*(Big &a, Big &b);
    friend Big operator/(Big &a, Big &b);
    friend Big operator%(Big &a, Big &b);

    friend bool operator>(Big &a, Big &b);
    friend bool operator<(Big &a, Big &b);
    friend bool operator>=(Big &a, Big &b);
    friend bool operator<=(Big &a, Big &b);
    friend bool operator==(Big &a, Big &b);
    friend bool operator!=(Big &a, Big &b);

    friend istream &operator>>(istream &in, Big &a);
    friend ostream &operator<<(ostream &out, Big &a);
};
