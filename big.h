#pragma once 
#include <iostream>
#include <cstring>
using namespace std;

typedef int BigError;
typedef unsigned int base;
typedef unsigned long long doubleBase;

namespace BigErrors {
		static const BigError OK = 0;
		static const BigError INCORRECT_SYMBOL = 1;
		static const BigError INCOMPATIBLE_OPERANDS = 2;
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

		int GetCapacity() const;
		int GetLength() const;
		void Resize(int); //количество блоков
		friend int Compare(const Big &b, const Big &a);

		Big& operator = (const Big &a); // this = a
		friend Big operator + (Big &b, Big &a);
		friend Big operator - (Big &b, Big &a); //b-a
		friend Big& operator / (Big &b, Big &a);
		friend Big& operator * (Big &a);
		friend istream& operator >> (istream &in, Big& a);
		friend ostream& operator << (ostream &out, Big& a);
};

