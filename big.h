#pragma once
#include <iostream>
#include <cstring>
using namespace std;

const int TEST_BOX = 100000;

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
private:
    base *al;  //в начале всегда
    base *ar;
    base *ah;  // range of allocation memory

public:
    Big();
    ~Big();
    Big(Big &);

    int Rand(int bound);
    int GetCapacity() const;
    int GetLength() const;
    void ResizeWithSave(int);
    void Resize(int);  //количество блоков
    void Compress();
    Big Mul(base);
    Big Div(base, base &);
    friend int Compare(const Big &b, const Big &a);
    friend int CompareWithZero(const Big &b);
    friend Big Substraction(Big &b, Big &a, int &flag);
    friend Big Division(Big &, Big &, Big &);

    Big &operator=(const Big &a);  // this = a
    friend Big operator+(Big &b, Big &a);
    friend Big operator-(Big &b, Big &a);  // b-a
    friend Big operator*(Big &b, Big &a);
    friend Big operator/(Big &b, Big &a);
    friend Big operator%(Big &b, Big &a);
    friend istream &operator>>(istream &in, Big &a);
    friend ostream &operator<<(ostream &out, Big &a);
};
