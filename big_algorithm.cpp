#include "big.h"

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
        r_dash.al[k+1] = 1;
        for(int i = r_dash.GetLength(); i < k + 1; i++) {
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
