#include "big.h"

using namespace BigErrors;

int main() {
		Big num;
		cin >> num;
		cout << num;
		Big num1;
		cin >> num1;
		cout << num1;
		int result;
		result = Compare(num,num1);
		cout << "lolmain" <<endl;	
		cout << result << endl;
		return 0;
}
