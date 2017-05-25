#include <time.h>
#include "big.h"
#include <stdlib.h>

using namespace BigErrors;

int main() {
		try {
				for(int i = 0; i < 100; i++) {
					cout << i << endl;
					Big num;
					Big num1;
					Big rem;
					Big result;
					Big result1;
					Big result_be;
					num.Rand(100001);
					num1.Rand(80);
					result = Division(num, num1, rem);
					result_be= result * num1; 
					result1 = result_be + rem;
					int t =  Compare(num, result1);

					if (t) {
						cout << "i = " << i << endl;
						cout << "t = " << t << endl;
						cout << "LENGTH result1 = " << result1.GetLength() << endl;
						cout << "LENGTH num = " << num.GetLength() << endl;
						cout << "Length result_be = " << result_be.GetLength() << endl;
						//cout << "num = " << num << endl;
						//cout << "num1 = " << num1 << endl;
					//	cout << "result = " << result << endl;
					  //	cout << "rem = " << rem << endl;
						//cout << "result_be = " << result_be << endl;
						//cout << "result1 = " << result1 << endl;
						return 0;						
					}; 
				}
			/*	Big num;
				Big num1;
				cin >> num;
				cin >> num1;
				Big result;
				Big rem;
				result = Division(num, num1, rem);
				cout << result << endl;
				cout << rem;*/
				return 0;
			} catch (...) {
				cout << "error";
			}
}
