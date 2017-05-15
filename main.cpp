#include <time.h>
#include "big.h"
#include <stdlib.h>

using namespace BigErrors;

int main() {
		try {
				Big num;
				Big num1;
				base r;
		//		num.Rand(110000);
		//		num1.Rand(110000);
				cin >> num;
				cin >> num1;
			//	cout << num;
			//	cout << num1;
				Big result;
			//	result = num.Div(num1, r);
				result = num / num1;
				cout << result << hex << r;
		//		Big result1;
		//		result1 = result - num;
			//	cout << result1;
			//	cout << "lol" << endl;
		//		cout << Compare(result1, num1) << endl;;
				return 0;
			} catch (...) {
				cout << "error from main" << endl;
			}
}
