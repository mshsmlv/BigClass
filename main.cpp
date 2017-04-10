#include "big.h"
#include <time.h>
#include <stdlib.h>

using namespace BigErrors;

int main() {
		Big num;
		Big num1;
//		num.Rand(110000);
//		num1.Rand(110000);
		cin >> num;
		cin >> num1;
		cout << num;
		cout << num1;
		Big result;
		result = num * num1;
		cout << result;
//		Big result1;
//		result1 = result - num;
	//	cout << result1;
		cout << "lol" << endl;
//		cout << Compare(result1, num1) << endl;;
		return 0;
}
