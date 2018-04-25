#include <stdlib.h>
#include <time.h>
#include "big.h"
const int test_range = 100;

using namespace BigErrors;

int main()
{
    /*
    Big num, num1, rem, result, result_be, q, w;

     int M = 1024;
     int T = 10000;
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
         q = num - rem;
         w = num1 * result;
     } while ((num == result_be) && (q == w) && (rem < num1) && T);

     if (T) {
         std::cout << "LENGTH num = " << num.GetLength() << std::endl;
         std::cout << "Length result_be = " << result_be.GetLength() << std::endl;
         std::cout << "num = " << num << std::endl;
         std::cout << "num1 = " << num1 << std::endl;
         std::cout << "result = " << result << std::endl;
         std::cout << "rem = " << rem << std::endl;
         std::cout << "result_be = " << result_be << std::endl;
         std::cout << "q = " << q <<std::endl;
         std::cout << "w = " << w << std::endl;
     };
*/
    Big x, z, mod, result, result1;
    int M = 10000;
    int T, T1;
    T = T1 = test_range;
    int t;
    /*
        while(T) {
          x.Rand(rand()%M);
          z.Rand(rand()%M);

          result = Karatsuba(x, z);
          result1 = x*z;
          if(result != result1) {
            std::cout << "T = " <<T << std::endl;
            std::cout << "x = " << x << std::endl;
            std::cout << "z = " << z << std::endl;
            std::cout << "result  = " << result << std::endl;
            std::cout << "result1 = " << result1 << std::endl;
            std::cout << "result_length  = " << result.GetLength() << std::endl;
            std::cout << "result1_length = " << result1.GetLength() << std::endl;
            return 0;
          }
          T--;
        }*/

/*    x.Rand(M);
    z.Rand(M);
    t = time(NULL);
    while (T) {
        Karatsuba(x, z);
        T--;
    }
    t = time(NULL) - t;
    std::cout << "Karatsuba time: " << t << std::endl;

    t = time(NULL);
    while (T1) {
        x *z;
        T1--;
    }
    t = time(NULL) - t;
    std::cout << "Mul time: " << t << std::endl;*/

    Big n;
    n.Rand(10);
    MillerRubin(n, 20);
}
