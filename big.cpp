#include "big.h"
#include <stdlib.h>

using namespace BigErrors;
const int DEBUG_MODE = 0;

Big ::Big()
{
    al = new base[1030];
    ah = al + 1030 - 1;
    ar = al;
}

Big ::~Big()
{
    if (al) {
        delete[] al;
        al = NULL;
    }
}

Big ::Big(Big &old_n)
{
    //	cout << "работает конструктор копирования" << endl;
    int capacity = old_n.ah - old_n.al + 1;
    int length = old_n.ar - old_n.al + 1;
    al = new base[capacity];
    ah = al + capacity - 1;
    ar = al + length - 1;
    for (int i = 0; i < length; i++) {
        al[i] = old_n.al[i];
    }
}

int Big ::Rand(int boundary)
{
    if (GetCapacity() < boundary) {
        Resize(TEST_BOX);
    }

    ar = al;
    for (int i = 0; i < boundary; i++) {
        al[i] = rand();
        ar++;
    }
    ar--;
    return 0;
}

int Big ::GetCapacity() const
{
    return ah - al + 1;
}

int Big ::GetLength() const
{
    return ar - al + 1;
}

// quantity of boxes
void Big ::Resize(int new_capacity)
{
    if (GetCapacity() < new_capacity) {
        if (al)
            delete[] al;
        al = new base[new_capacity];
        ah = al + new_capacity - 1;
        ar = al;
    }
}

// aborting insignificant zeros
void Big ::Compress()
{
    for (int i = GetLength() - 1; 0 < i; i--) {
        if (al[i] != 0) {
            return;
        }
        ar--;
    }
}

Big Big ::Mul(base small)
{
    Big result;

    if (0 == small) {
        result.al[0] = 0;
        result.ar = result.al;
        return result;
    }
    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);

    if (result.GetCapacity() < GetLength() + 1) {
        result.Resize(GetLength() + 1);
    }

    doubleBase cup;
    doubleBase carry = 0;
    result.ar = result.al;

    int i;
    for (i = 0; i < GetLength(); i++) {
        cup = static_cast<doubleBase>(al[i]) * static_cast<base>(small) + carry;
        carry = cup / mask;
        result.al[i] = static_cast<base>(cup % mask);
        result.ar++;
    }

    result.al[i] = carry;
    result.Compress();
    return result;
}

Big Big ::Div(base small, base &remainder)
{
    Big result;
    if (0 == small) {
        throw DIV_ZERO;
    }

    if (GetLength() <= 1 && 0 == al[0]) {  // comparison with zero
        result.ar = result.al;
        result.al[0] = 0;
        remainder = small;
        return result;
    }

    result.Resize(GetCapacity());

    doubleBase t = 0;
    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);
    remainder = 0;

    result.ar = result.al + GetLength() - 1;
    for (int i = 0; i < result.GetLength(); i++) {
        result.al[i] = 0;
    }

    for (int i = GetLength() - 1; 0 <= i; i--) {
        t = static_cast<doubleBase>(al[i]) + static_cast<doubleBase>(remainder) * mask;

        result.al[i] = static_cast<base>(t / small);
        remainder = static_cast<base>(t % static_cast<doubleBase>(small));
    }

    result.Compress();
    return result;
}

/*1 - больше
 * -1 меньше
 * 0 - равно*/

int Compare(const Big &b, const Big &a)
{
    if (b.GetLength() > a.GetLength()) {
        return 1;
    } else if (b.GetLength() < a.GetLength()) {
        return -1;
    }

    for (int i = a.GetLength() - 1; 0 <= i; i--) {
        if (b.al[i] > a.al[i]) {
            return 1;
        } else if (b.al[i] < a.al[i]) {
            return -1;
        }
    }

    return 0;
}

int CompareWithZero(const Big &a)
{
    for (int i = a.GetLength() - 1; 0 <= i; i--) {
        if (a.al[i] != 0) {
            return false;
        }
    }
    return true;
}

Big &Big ::operator=(const Big &a)
{
    if (GetCapacity() != a.GetCapacity()) {
        Resize(a.GetCapacity());
    }

    ar = al;
    int length = a.GetLength();

    for (int i = 0; i < length; i++) {
        al[i] = a.al[i];
        ar++;
    }
    ar--;
}

Big operator+(Big &b, Big &a)
{
    Big result;
    doubleBase glass;  // for the overflow:)
    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);
    int carry = 0;  // at the begin of addition
    int BLength = b.GetLength();
    int ALength = a.GetLength();
    int LessLength;
    int rcapacity;

    if (ALength <= BLength) {
        LessLength = ALength;
    } else {
        LessLength = BLength;
    }

    if (a.GetCapacity() <= b.GetCapacity()) {
        rcapacity = b.GetCapacity();
    } else {
        rcapacity = a.GetCapacity();
    }

    if (rcapacity + 1 > result.GetCapacity()) {
        result.Resize(rcapacity + 1);
    }

    result.ar = result.al;

    int i;
    for (i = 0; i < LessLength; i++) {
        glass = static_cast<doubleBase>(b.al[i]) + static_cast<doubleBase>(a.al[i]) + carry;
        result.al[i] = glass % mask;
        carry = !!(glass / mask);  // for the next digit
        result.ar++;
    }
    // add tail from array, which longer
    if (i < ALength) {
        for (i = i; i < ALength; i++) {
            glass = a.al[i] + carry;
            result.al[i] = glass % mask;
            result.ar++;
            carry = !!(glass / mask);
        }
    }

    else if (i < BLength) {
        for (i = i; i < BLength; i++) {
            glass = b.al[i] + carry;
            result.al[i] = glass % mask;
            result.ar++;
            carry = !!(glass / mask);
        }
    }

    if (carry) {  // digit carry in
        result.al[i] = carry;
        result.ar++;
    }
    result.ar--;
    result.Compress();
    return result;
}

Big operator-(Big &b, Big &a)
{
    int flag = Compare(b, a);
    if (-1 == flag) {
        cout << "incompatible_operands" << endl;
        throw INCOMPATIBLE_OPERANDS;
    }

    Big result;
    result.Resize(b.GetCapacity());
    result.ar = result.al;

    // if they are equal
    if (0 == flag) {
        result.al[0] = 0;
        return result;
    }

    int carry = 0;
    int given = 0;
    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);  //ЗДЕСЬ МАГИЯ!!!
    doubleBase cup = 0;
    doubleBase glass;

    int i;

    for (i = 0; i < a.GetLength(); i++) {
        result.ar++;
        cup = a.al[i] + carry;

        if (static_cast<doubleBase>(b.al[i]) < cup) {
            given = 1;
        }

        if (given) {
            glass = static_cast<doubleBase>(b.al[i]) + mask - cup;
            result.al[i] = static_cast<base>(glass);
            carry = 1;
            given = 0;
        }

        else {
            result.al[i] = b.al[i] - static_cast<base>(cup);
            carry = 0;
            given = 0;
        }
    }

    for (i; i < b.GetLength(); i++) {
        result.ar++;
        if (static_cast<doubleBase>(b.al[i]) < carry) {
            given = 1;
        }

        if (given) {
            glass = static_cast<doubleBase>(b.al[i]) + static_cast<doubleBase>(mask) - static_cast<doubleBase>(carry);
            result.al[i] = glass;
            carry = 1;
            given = 0;
        }

        else {
            result.al[i] = static_cast<doubleBase>(b.al[i]) - carry;
            carry = 0;
            given = 0;
        }
    }
    result.ar--;
    result.Compress();
    return result;
}

Big operator*(Big &b, Big &a)
{
    Big result;
    if (a.GetLength() <= 1) {
        result = b.Mul(a.al[0]);
        return result;
    }
    result.Resize(b.GetLength() + a.GetLength());

    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);
    doubleBase cup;
    doubleBase carry = 0;

    // filling with zeros
    for (int i = 0; i < result.GetCapacity(); i++) {
        result.al[i] = 0;
        result.ar++;
    }
    result.ar--;

    int i, j;
    for (j = 0; j < a.GetLength(); j++) {
        if (0 == a.al[j]) {
            continue;
        }

        for (i = 0; i < b.GetLength(); i++) {
            cup = static_cast<doubleBase>(b.al[i]) * static_cast<doubleBase>(a.al[j]) +
                static_cast<doubleBase>(result.al[i + j]) + carry;
            result.al[i + j] = static_cast<base>(cup);
            carry = cup >> sizeof(base) * 8;
        }
        result.al[i + j] = carry;
        carry = 0;
    }
    result.Compress();
    return result;
}

// e/c
Big Division(Big &e, Big &c, Big &remainder)
{
    Big a, b, result;

    a = e;
    b = c;

    int j = 0;
    int n = b.GetLength();
    int m = a.GetLength() - n;
    int flag;  //для 4 шага

    a.Resize(a.GetLength() + 1);

    if (DEBUG_MODE) {
        std::cout << "a = 0x" << a << std::endl;
        std::cout << "b = 0x" << b << std::endl;
    }

    if (CompareWithZero(e)) {
        result.ar = result.al + 1;
        result.al[0] = 0;
        remainder.ar = remainder.al + 1;
        remainder.al[0] = 0;
        return result;
    }

    if (-1 == Compare(e, c)) {
        result.ar = result.al;
        result.al[0] = 0;
        remainder = e;
        return result;
    }

    //если деление на базу
    if (b.GetLength() <= 1) {
        base r;
        result = a.Div(b.al[0], r);
        remainder.al[0] = r;
        remainder.ar = remainder.al;
        return result;
    }

    doubleBase d, mask;
    mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);
    d = mask / static_cast<doubleBase>(b.al[b.GetLength() - 1] + 1);

    /*NORMALIZATION*/
    //считаем d
    if (DEBUG_MODE) {
        std::cout << "0x" << std::hex << d << " = 0x" << mask << "/"
                  << "(0x" << std::hex << b.al[b.GetLength() - 1] << "+1)" << std::endl;
    }

    a = a.Mul(static_cast<base>(d));
    if (a.GetLength() == e.GetLength()) {
        if (DEBUG_MODE) {
            std::cout << "d = 1" << std::endl;
        }
        a.ar++;
        a.al[a.GetLength() - 1] = 0;
    }
    b = b.Mul(static_cast<base>(d));

    if (DEBUG_MODE) {
        std::cout << "NORMALIZATION:" << std::endl;
        std::cout << "a = 0x" << a << std::endl;
        std::cout << "b = 0x" << b << std::endl << std::endl;

        std::cout << "начинаем делить:" << std::endl;
        std::cout << "n = " << n << std::endl;
        std::cout << "m = " << m << std::endl;
    }

    doubleBase roof, left_part, right_part;
    Big glass, new_num, q;

    q.ar = q.al + m + 1;

    for (int i = 0; i < q.GetLength(); i++) {
        q.al[i] = 0;
    }

    base b1 = b.al[b.GetLength() - 1];
    base b2 = b.al[b.GetLength() - 2];

    if (DEBUG_MODE) {
        std::cout << "b1 = 0x" << std::hex << b1 << std::endl;
        std::cout << "b2 = 0x" << std::hex << b2 << std::endl;
    }

    base aj, aj1, aj2;
    for (j = 0; j <= m; j++) {
        aj = a.al[a.GetLength() - j - 1];
        aj1 = a.al[a.GetLength() - j - 1 - 1];
        aj2 = a.al[a.GetLength() - j - 1 - 2];

        if (DEBUG_MODE) {
            std::cout << "====================================" << std::endl;
            std::cout << "j = " << j << std::endl;
            std::cout << "a[j] = " << aj << std::endl;
            std::cout << "a[j+1] = " << aj1 << std::endl;
            std::cout << "a[j+2] = " << aj2 << std::endl;
        }

        if (aj == b1) {
            roof = mask - 1;
        } else {
            if (DEBUG_MODE) {
                std::cout << "(0x" << std::hex << aj << "*0x" << std::hex << mask << "+0x" << std::hex << aj1 << ")/0x"
                          << std::hex << b1 << std::endl;
            }
            roof = (static_cast<doubleBase>(aj) * static_cast<doubleBase>(mask) + static_cast<doubleBase>(aj1)) /
                static_cast<doubleBase>(b1);
            if (DEBUG_MODE) {
                std::cout << "=0x" << std::hex << roof << std::endl;
            }
        }

        //проверка неравенства

        while (1) {
            left_part = static_cast<doubleBase>(b2) * roof;

            right_part =
                static_cast<doubleBase>(aj) * mask + static_cast<doubleBase>(aj1) - roof * static_cast<doubleBase>(b1);
            /*дальнейшее умножение приведет к переполнению,
             * а left_part переполнится никогда не может =>
             * нервенство не выполнено*/
            if (right_part >> sizeof(base) * 8) {
                break;
            }
            right_part = right_part * mask + static_cast<doubleBase>(aj2);
            if (left_part > right_part) {
                roof--;
                if (DEBUG_MODE) {
                    std::cout << "roof--" << std::endl;
                }
            } else
                break;
        }

        if (DEBUG_MODE) {
            std::cout << "roof = 0x" << std::hex << roof << std::endl;
        }

        //умножить и вычесть

        new_num.ar = new_num.al;

        for (int i = 0; i <= n; i++) {
            new_num.al[n - i] = a.al[a.GetLength() - j - 1 - i];
            new_num.ar++;
        }

        if (DEBUG_MODE) {
            std::cout << "new_num = 0x" << new_num << std::endl;
        }

        flag = 0;
        glass = b.Mul(static_cast<base>(roof));
        while (-1 == Compare(new_num, glass)) {
            cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
            cout << "roof " << hex << roof << endl;
            roof--;
            flag = 1;
            //	cout << "glas --- " << endl;
            cout << "glass = " << glass << endl;
            glass = b.Mul(static_cast<base>(roof));
            cout << "glass1 = " << glass << endl;

            cout << "new_num  = " << new_num << endl;
            cout << "roof1  = " << hex << roof << endl;

            cout << "b = " << b << endl;
            cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
            exit(0);
        }

        int stored_length_new_num = new_num.GetLength();
        new_num = new_num - glass;

        if (stored_length_new_num > new_num.GetLength()) {
            for (int i = new_num.GetLength(); i < stored_length_new_num; i++) {
                new_num.al[i] = 0;
                new_num.ar++;
            }
        }

        if (DEBUG_MODE) {
            std::cout << "после вычитания new_num:" << std::endl;
            std::cout << "0x" << new_num << std::endl;
        }

        for (int i = 0; i <= n; i++) {
            a.al[a.GetLength() - j - 1 - i] = new_num.al[n - i];
        }

        if (DEBUG_MODE) {
            std::cout << "переписали new_num в a:" << std::endl;
            std::cout << "0x" << a << std::endl;
        }

        if (flag) {
            if (DEBUG_MODE) {
                std::cout << "шаг 4 был отрицательный" << std::endl;
            }
            roof--;
            a = a + b;
        }
        q.al[m - j] = static_cast<base>(roof);
    }

    base r;
    a = a.Div(d, r);
    q.Compress();

    if (DEBUG_MODE) {
        std::cout << "частное = 0x" << q << std::endl;
        std::cout << "остаток = 0x" << a << std::endl;
    }

    // std::cout << "отладка богов" << std::endl;
    remainder = a;
    return q;
}

ostream &operator<<(ostream &out, Big &a)
{
    int block = sizeof(base) * 2;  // *8/4 how many numbers in the "base"
    int length = a.GetLength();
    int mask = 0xF;
    char tmp;
    unsigned int flag = 1;

    for (int i = a.GetLength() - 1; 0 <= i; i--) {  // starting from the older
        for (int l = (block - 1) * 4; l >= 0; l -= 4) {
            tmp = (a.al[i] & (mask << l)) >> l;  // get an each number from the block(one number - four bytes)
            if (tmp >= 0 && tmp <= 9) {
                if (flag && 0 == tmp) {
                    continue;
                }
                flag = 0;
                tmp = tmp + '0';
                out << tmp;
            } else if (tmp >= 0xA && tmp <= 0xF) {
                tmp = tmp + 87;
                out << tmp;
                flag = 0;
            } else
                throw INCORRECT_SYMBOL;
        }
    }

    if (flag) {
        out << "0";
    }
    return out;
}

istream &operator>>(istream &in, Big &a)
{
    int block = sizeof(base) * 2;  // *8 / 4
    base tmp_0, tmp_1;             // tmp 0 | tmp_1 -> al[i]
    int index;
    string string_for_num;
    in >> string_for_num;
    int length_s = string_for_num.length();
    int n = (length_s) / block + !!((length_s) % (block));
    if (n > a.GetCapacity()) {
        a.Resize(n);
    }
    for (int k = 0; k < n; k++) {
        tmp_0 = 0;
        tmp_1 = 0;
        for (int i = 0; i < block; i++) {
            index = length_s - k * block - i - 1;
            if (index < 0)  // control start of the string
                break;
            char symbol = string_for_num[index];
            if (symbol >= '0' && symbol <= '9') {
                tmp_1 = symbol - 48;
            } else if (symbol >= 'a' && symbol <= 'f') {
                tmp_1 = symbol - 87;  // 10 + symbol - 97;
            } else if (symbol >= 'A' && symbol <= 'F') {
                tmp_1 = symbol - 55;  // symbol + 10 - 65
            } else
                throw INCORRECT_SYMBOL;
            tmp_0 = tmp_0 | (tmp_1 << 4 * i);
        }
        a.al[k] = tmp_0;
        a.ar++;  // changing right range
    }
    a.ar--;
    return in;
}
