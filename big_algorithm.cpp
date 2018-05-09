#include "big.h"
#include <stdlib.h>
const int range = 50;

Big GetZForBurretReduction(Big &mod)
{
    Big z;

    int k = mod.GetLength();
    k = 2 * k;
    z.Resize(k + 1);

    z.al[k] = 1;
    z.ar = z.al + k;
    for (int i = 0; i < k; i++) {
        z.al[i] = 0;
    }

    for (int i = 0; i < k; i++) {
        z.al[i] = 0;
    }
    z = z / mod;

    return z;
}

Big BurretReduction(Big &x, Big &mod, Big &z)
{
    Big q, r1, r2, r_dash, glass;

    if (x < mod) {
        return x;
    }

    int k = mod.GetLength();

    r1.Resize(k + 1);
    r2.Resize(k + 1);
    q.Resize(x.GetLength() * 2 * k + 1);

    // q = x/bk_1
    int i, j;
    for (i = k - 1, j = 0; i < x.GetLength(); i++, j++) {
        q.al[j] = x.al[i];
    }
    q.ar = q.ar + x.GetLength() - k;

    glass = q * z;

    //  q = q / bk1;
    for (i = k + 1, j = 0; i < glass.GetLength(); i++, j++) {
        q.al[j] = glass.al[i];
    }
    q.ar = q.al + glass.GetLength() - (k + 2);

    // r1 = x % bk1;
    r1.ar = r1.al;
    for (i = 0; i < k + 1 && i < x.GetLength(); i++) {
        r1.al[i] = x.al[i];
        r1.ar++;
    }
    r1.ar--;

    glass = q * mod;

    // r2 = glass % bk1;
    r2.ar = r2.al;
    for (i = 0; i < k + 1 && i < glass.GetLength(); i++) {
        r2.al[i] = glass.al[i];
        r2.ar++;
    }
    r2.ar--;

    if (r1 >= r2) {
        r_dash = r1 - r2;
    } else {
        r_dash.al[k + 1] = 1;
        for (int i = r_dash.GetLength(); i < k + 1; i++) {
            r_dash.al[i] = 0;
        }
        r_dash.ar = r_dash.al + k + 1;
        r_dash = r_dash - r2;
    }

    while (r_dash >= mod) {
        r_dash = r_dash - mod;
    }
    return r_dash;
}

Big Karatsuba(Big &u, Big &v)
{
    int u_length = u.GetLength();
    int v_length = v.GetLength();

    if (u_length < range || v_length < range) {
        return u * v;
    }

    int max_length = max(u_length, v_length);
    int n = max_length / 2 + (!!(max_length % 2));

    doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base) * 8);
    doubleBase glass;

    Big u1, u0, v1, v0, a, b, c, left_bracket, right_bracket, result;
    if (CompareWithZero(u) || CompareWithZero(v)) {
        result.al[0] = 0;
        result.ar = result.al;
        return result;
    }

    u1.Resize(u_length);
    u0.Resize(u_length);
    v1.Resize(v_length);
    v0.Resize(v_length);
    a.Resize(u_length + v_length);
    b.Resize(u_length + v_length);
    c.Resize(u_length + v_length);
    left_bracket.Resize(u_length + v_length);
    right_bracket.Resize(u_length + v_length);
    result.Resize(u_length + v_length);

    int i, j;
    for (i = 0; i < n && i < u_length; i++) {
        u0.al[i] = u.al[i];
    }
    u0.ar = u0.al + i - 1;

    if (n >= u_length) {
        u1.al[0] = 0;
        v1.ar = v1.al;
    } else {
        for (i = n, j = 0; i < u_length; i++, j++) {
            u1.al[j] = u.al[i];
        }
        u1.ar = u1.al + i - n - 1;
    }

    for (i = 0; i < n && i < v_length; i++) {
        v0.al[i] = v.al[i];
        v0.ar++;
    }
    v0.ar = v0.al + i - 1;

    if (n >= v_length) {
        v1.al[0] = 0;
        v1.ar = v1.al;
    } else {
        for (i = n, j = 0; i < v_length; i++, j++) {
            v1.al[j] = v.al[i];
        }
        v1.ar = v1.al + i - n - 1;
    }

    a = Karatsuba(u1, v1);
    b = Karatsuba(u0, v0);

    left_bracket = u1 + u0;
    right_bracket = v1 + v0;
    c = Karatsuba(left_bracket, right_bracket);
    c = c - a;
    c = c - b;

    result.ar = result.al + v_length + u_length - 1;
    for (i = 0; i < b.GetLength(); i++) {
        result.al[i] = b.al[i];
    }
    for (i = 2 * n, j = 0; j < a.GetLength(); i++, j++) {
        result.al[i] = a.al[j];
    }

    doubleBase carry = 0;

    for (i = n, j = 0; j < c.GetLength(); i++, j++) {
        glass = static_cast<doubleBase>(result.al[i]) + static_cast<doubleBase>(c.al[j]) + carry;
        result.al[i] = glass;
        carry = !!(glass / mask);
    }

    while (carry) {
        glass = static_cast<doubleBase>(result.al[i]) + carry;
        result.al[i] = glass;
        carry = !!(glass / mask);
        i++;
    }

    result.Compress();
    return result;
}

bool MillerRubin(Big &n, int t) {

    n.al[0] |= 1;
    int s; 
    base remainder;
    Big r, n_1, one, glass, b, y, two;
    one.al[0] = 1;
    one.ar = one.al;

    two.al[0] = 2;
    two.ar = two.al;

    n_1 = n - one;
    r = n_1;

    for(s = 0;; s++) {
        glass = r.Div(2, remainder);
        if(remainder) break;
        r = glass;
    }

    while(t) {

        b.al[0] = rand()|1;
        b.ar = b.al;

        y = Degree(b, r, n);

        if((y.al[0] != 1) && y != n_1) {
            int j = 1;
            while(j < s) {
                if(y == n_1) {
                    break;
                }
                y = Degree(y, two, n);
                if((y.al[0] == 1) && (y.ar == y.al)) {
                    return false;
                }
                j++;
            }
            if(y != n_1) {
                return false;
            }
        }
        t--;
    }
    return true;
}
