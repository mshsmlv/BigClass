#include <iostream>
#include <cstring>
using namespace std;

typedef int BigError;
typedef unsigned int base;

namespace BigErrors {
		static const BigError OK = 0;
		static const BigError INCORRECT_SYMBOL = 1;
};

class Big {
	private:
		base* al; //в начале всегда 
		base* ar; 
		base* ah; //range of allocation memory
	
	public:
		Big();
		~Big();
		Big(Big&);

		int GetCapacity();
		int GetLength();
		void Resize(int); //количество блоков
	
		Big& operator = (Big &a); // this = a
		friend Big& operator + (Big &b, Big &a);
		friend Big& operator - (Big &a);
		friend Big& operator / (Big &b, Big &a);
		friend Big& operator * (Big &a);
		friend istream& operator >> (istream &in, Big& a);
		friend ostream& operator << (ostream &out, Big& a);
};


