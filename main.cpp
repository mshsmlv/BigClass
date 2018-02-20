#include <stdlib.h>
#include <time.h>
#include "big.h"

using namespace BigErrors;

int main()
{
    Big num;
    Big num1;
    Big rem;
    Big result;
    Big result1;
    Big result_be;
    int M = 5;
    // srand(time(NULL));
    for (int i = 0; i < 10000; i++) {
        // std::cout << "***********************" << std::endl;
        cout << i << endl;

        int n = rand() % M + 1;
        int m = rand() % M + 1;

        //     cin >> num;
        //   cin >> num1;
        num.Rand(n);
        num1.Rand(m);

        if (CompareWithZero(num1))
            continue;

        result = Division(num, num1, rem);
        //  std::cout << result;
        result_be = num1 * result;
        result_be = result_be + rem;
        int t = Compare(num, result_be);

        if (t) {
            //	cout << "i = " << i << endl;
            cout << "t = " << t << endl;
            cout << "LENGTH result1 = " << result1.GetLength() << endl;
            cout << "LENGTH num = " << num.GetLength() << endl;
            cout << "Length result_be = " << result_be.GetLength() << endl;
            cout << "num = " << num << endl;
            cout << "num1 = " << num1 << endl;
            cout << "result = " << result << endl;
            cout << "rem = " << rem << endl;
            cout << "result_be = " << result_be << endl;
            // cout << "result1 = " << result1 << endl;
            return 0;
        };

        Big q, w;
        q = num - rem;
        w = num1 * result;
        t = Compare(q, w);
        if (t) {
            cout << "t = " << t << endl;
            cout << "LENGTH result1 = " << result1.GetLength() << endl;
            cout << "LENGTH num = " << num.GetLength() << endl;
            cout << "Length result_be = " << result_be.GetLength() << endl;
            cout << "num = " << num << endl;
            cout << "num1 = " << num1 << endl;
            cout << "result = " << result << endl;
            cout << "rem = " << rem << endl;
            cout << "result_be = " << result_be << endl;
            cout << "num - rem = " << q << endl;
            cout << "num1*result = " << w << endl;
            std::cout << "второе условие" << std::endl;
            return 0;
        }

        t = Compare(rem, num1);
        if (-1 != t) {
            std::cout << "третье условие" << std::endl;
            return 0;
        }
    }
    return 0;
}
