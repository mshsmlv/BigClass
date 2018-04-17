#include <stdlib.h>
#include <time.h>
#include "big.h"

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
    int M = 35;
    int M1 = 4000;
    int T = 500;

    while(T) {
      x.Rand(M);
      z.Rand(M);

      result = Karatsuba(x, z);
      result1 = x*z;
      if(result != result1) {
        std::cout << "x = " << x << std::endl;
        std::cout << "z = " << z << std::endl;
        std::cout << "result  = " << result << std::endl;
        std::cout << "result1 = " << result1 << std::endl;
        std::cout << "result_length  = " << result.GetLength() << std::endl;
        std::cout << "result1_length = " << result1.GetLength() << std::endl;
        return 0;
      }
      T--;
    }
}
