#include <stdlib.h>
#include <time.h>
#include "big.h"

using namespace BigErrors;

int main()
{
    Big num, num1, rem, result, result_be;

    int M = 1024;
    int T = 1000;
    srand(time(NULL));

    do {
        std::cout << T << std::endl;

        int n = rand() % M + 1;
        int m = rand() % M + 1;

        num.Rand(n);
        num1.Rand(m);

        if (CompareWithZero(num1))
            continue;

        result = Division(num, num1, rem);
        result_be = num1 * result;
        result_be = result_be + rem;
        T--;
    } while ((Compare(num, result_be) == 0) && (Compare(num - rem, num1 * result) == 0) && (Compare(rem, num1) == -1) &&
             T);

    if (T) {
        std::cout << "LENGTH num = " << num.GetLength() << std::endl;
        std::cout << "Length result_be = " << result_be.GetLength() << std::endl;
        std::cout << "num = " << num << std::endl;
        std::cout << "num1 = " << num1 << std::endl;
        std::cout << "result = " << result << std::endl;
        std::cout << "rem = " << rem << std::endl;
        std::cout << "result_be = " << result_be << std::endl;
    };

    return 0;
}
