#include "big.h"
#include <stdlib.h>

//#include <string.h>

using namespace BigErrors;
typedef unsigned int base;

Big :: Big() {
		al = new base[100];
		ah = al + 100 - 1;
		ar = al;
}

Big :: ~Big() {
		if(al)
			delete [] al;
}

Big :: Big(Big& old_n) {
		int capacity = old_n.ah - old_n.al;
		int length = old_n.ar - old_n.al;
		al = new base[capacity];
		ah = al + capacity;
		ar = al + length;
}

int Big :: Rand(int boundary) {
	
	if(GetCapacity() < boundary) {
		Resize(TEST_BOX);	
	}

	ar = al;
	for(int i = 0; i < boundary; i++) {
		al[i] =  rand();
		ar++;
	}
	return 0;

}

int Big :: GetCapacity() const {
		return ah - al + 1;
}

int Big :: GetLength() const {
		return ar - al;
}

//quantity of boxes
void Big :: Resize(int new_capacity) {
		if(this -> GetCapacity() < new_capacity) {
				if(al) 
					delete [] al;
				al = new base[new_capacity];
				ah = al + new_capacity; //??????????!!!!!!
				ar = al;
		}
}

//aborting insignificant zeros
void Big :: Compress() {
	for(int i = GetLength()-1; 0 < i; i--) {
		if(al[i] != 0) {
			return;
		}
		ar--;
	}
}

Big Big :: Imul(base small) {
	Big result;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	
	if(result.GetCapacity() < this->GetLength() + 1) {
		result.Resize(GetLength()+1);
	}

	doubleBase cup;
	doubleBase carry = 0;
	result.ar=result.al;

	int i;
	cout << "i=" << GetLength() << endl;
	for(i=0; i<GetLength(); i++) {
		cup = static_cast<doubleBase>(al[i])*static_cast<base>(small) + carry;
		carry = cup / mask;
		result.al[i] =  static_cast<base>(cup%mask);
		result.ar++;
	}
	result.al[i] = carry;
	result.ar++;
	result.Compress();
	return result;
}

int Compare(const Big &b, const Big &a) {
	if(b.GetLength() > a.GetLength()) {
		return 1;
	}
	else if(b.GetLength() < a.GetLength()) {
			return -1;
		}

	for(int i = a.GetLength()-1; 0 <= i; i--) {
		if(b.al[i] > a.al[i]) {
			return 1;
		}
		else if (b.al[i] < a.al[i]) {
			return -1;
		}
	}
	
	return 0;
}

Big& Big ::  operator = (const Big &a) {
	if(this -> GetCapacity() != a.GetCapacity()) {
		this -> Resize(a.GetCapacity());
	}
	
	this -> ar = this -> al;
cout << this -> GetLength() << endl;
	int length = a.GetLength();
	cout << length << endl;
	
	for(int i=0; i<length; i++) {
		this -> al[i] = a.al[i];
		this -> ar++;
	}
}

Big operator + (Big &b, Big &a) {
	Big result;
	doubleBase glass;// for the overflow:)
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	int carry = 0; //at the begin of addition
	int BLength = b.GetLength();
	int ALength = a.GetLength();
	int LessLength;
	int rcapacity;

	if (ALength <= BLength) {
		LessLength = BLength;
	}
	else {
		LessLength = ALength;
	}

	if(a.GetCapacity() <= b.GetCapacity()) {
		rcapacity = b.GetCapacity();
	}
	else {
		rcapacity = a.GetCapacity();
	}
	
	if(rcapacity+1 > result.GetCapacity()) {
		result.Resize(rcapacity + 1);
	}

	result.ar = result.al;

	int i;
	for(i=0; i<LessLength; i++) {
		glass = static_cast<doubleBase>(b.al[i]) + static_cast<doubleBase>(a.al[i]) + carry;
		result.ar++;
		result.al[i] = glass % mask;
		carry = !!(glass / mask); //for the next digit
	}
	//add tail from array, which longer
	if(i < ALength) {
		for(i = ++i; i < ALength; i++) {
			glass = a.al[i] + carry;
			result.al[i] = glass % mask;
			result.ar++;
			carry = !!(glass / mask);
			i++;
			result.al[i] = a.al[i] + carry;
		}
	}

	else if(i < BLength){
		for(i = ++i; i < BLength; i++) { 
			glass = b.al[i] + carry;
			result.al[i] = glass % mask;
			result.ar++;
			carry = !!(glass / mask);
			i++;
			result.al[i] = b.al[i] + carry;
		}
	}


	if (carry) {//digit carry in
			result.al[i] = carry;
			result.ar++;
	}

	result.Compress();
	return result;
}


Big operator - (Big &b, Big &a) {
	int flag = Compare(b, a);
	if(-1 == flag) {
		throw INCOMPATIBLE_OPERANDS;
	}
	
	Big result;
	result.Resize(b.GetCapacity());
	result.ar = result.al;
	
	//if they are equal
	if(0 == flag) {
		result.ar++;
		result.al[0] = 0;
		return result;
	}

	int carry = 0;
	int given = 0;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8); //ЗДЕСЬ МАГИЯ!!!
	doubleBase cup = 0;
	doubleBase glass;

	int i;
	for(i=0; i < a.GetLength(); i++) {

		result.ar++;
		cup = a.al[i] + carry;
		
		if(static_cast<doubleBase>(b.al[i]) < cup) {
			given = 1;
		}
		
		if(given) {
			glass  = static_cast<doubleBase>(b.al[i]) + 
						mask -
							cup; 
			result.al[i] = glass;
			carry = 1;
			given = 0;
		}

		else {
			result.al[i] = static_cast<doubleBase>(b.al[i]) - cup;
		}
	}
	
	for(i; i < b.GetLength(); i++) {

		result.ar++;
		if(static_cast<doubleBase>(b.al[i]) < carry) {
			given = 1;
		}

		if(given) {
			glass = static_cast<doubleBase>(b.al[i]) + 
						static_cast<doubleBase>(mask) - 
							static_cast<doubleBase>(carry);
			result.al[i] = glass;
			carry = 1;
			given = 0;
		}

		else {
			result.al[i] = static_cast<doubleBase>(b.al[i]) - carry;
		}
	}
	result.Compress();
	return result;
}

Big operator * (Big &b, Big &a) {
	
	Big result;
	if(a.GetLength() <= 1) {
		result = b.Imul(a.al[0]);
		return result;
	}
	result.Resize(b.GetLength() + a.GetLength());

	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	doubleBase cup;
	doubleBase carry = 0;

	//filling with zeros
	for(int i=0; i<result.GetCapacity(); i++) {
		result.al[i] = 0;
		result.ar++;
	}

	int i, j;
	for (j = 0; j < a.GetLength(); j++) { 
		
		if(0 == a.al[j]) {
			continue;
		}

		for(i = 0; i < b.GetLength(); i++) {
			cup = static_cast<doubleBase>(b.al[i])*static_cast<doubleBase>(a.al[j]) +
					static_cast<doubleBase>(result.al[i+j]) + carry;
			result.al[i+j] = static_cast<base>(cup);
			carry = cup >> sizeof(base)*8;
		}
		result.al[i+j] = carry;
		carry = 0;
	}
	result.Compress();
	return result;

}

ostream& operator << (ostream &out, Big &a) { 
	int block = sizeof(base)*2; // *8/4 how many numbers in the "base"
	int length = a.GetLength();
	int mask = 0xF;
	char tmp;
	out << "0x";
	for(int i = length-1; i >= 0; i--) { //starting from the older
		for(int l = (block-1)*4; l >= 0; l-=4) {
			tmp = (a.al[i] & (mask << l)) >> l ; //get an each number from the block(one number - four bytes) 
			if(tmp >= 0 && tmp <= 9) {
				tmp = tmp + '0';
				out << tmp;
			} 
			else if(tmp >= 0xA && tmp <= 0xF) {
				tmp = tmp + 87;
				out << tmp; 
			}
			else cout << "dich" << endl;
		}
	}
	out << endl;
}

istream& operator >> (istream &in, Big &a) {
	int block = sizeof(base)*2; // *8 / 4
	base tmp_0, tmp_1; //tmp 0 | tmp_1 -> al[i]
	int index;
	string string_for_num;
	cout << "Input num in 16:" << endl;
	in >> string_for_num;
	int length_s = string_for_num.length(); // includes "0x"
	int n = (length_s-2) / block + !!((length_s-2) % (block));
	if (n > a.GetCapacity()) { 
		a.Resize(n);
	} 
	for(int k=0; k<n; k++) {
		tmp_0 = 0;
		tmp_1 = 0;
		for(int i=0; i < block; i++) {
			index = length_s - k*block - i - 1;
			if(index < 2) //control start of the string 
				break;
			char symbol = string_for_num[index]; 
			if(symbol >= '0' && symbol <= '9') {
				tmp_1 = symbol - 48;
			} 
			else 
				if(symbol >= 'a' && symbol <= 'f') {
				tmp_1 = symbol - 87; //10 + symbol - 97;
				}
			else 
				if(symbol >= 'A' && symbol <= 'F') {
					tmp_1 = symbol - 55; //symbol + 10 - 65
				} 
				else throw INCORRECT_SYMBOL;
			tmp_0 = tmp_0 | (tmp_1 << 4*i);
		}
		a.al[k] = tmp_0;
		a.ar++; //changing right range
	}
}
