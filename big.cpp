#include "big.h"
#include <string.h>

using namespace BigErrors;
typedef unsigned int base;

Big :: Big() {
		al = new base[100];
		ah = al + 100 - 1;
		ar = al;
}

Big :: ~Big() {
		if(al)
			delete al;
}

Big :: Big(Big& old_n) {
		int capacity = old_n.ah - old_n.al;
		int length = old_n.ar - old_n.al;
		al = new base[capacity];
		ah = al + capacity;
		ar = al + length;
}

int Big :: GetCapacity() {
		return ah - al + 1;
}

int Big :: GetLength() {
		return ar - al + 1;
}

void Big :: Resize(int new_capacity) {
		if(this -> GetCapacity() < new_capacity) {
				if(al) 
					delete [] al;
				al = new base[new_capacity];
				ar = al;
		}
}

Big& Big ::  operator = (Big &a) { 
	if(this -> GetCapacity() != a.GetCapacity()) {
		this -> Resize(a.GetCapacity());
	}
	
	this -> ar = this -> al;	
	int length = a.GetLength();
	
	for(int i=0; i<length; i++) {
		this -> al[i] = a.al[i];
		this -> ar++;
	}
}

Big& operator + (Big &b, Big &a) {
	Big result;
	doubleBase glass;// for the overflow:)
	base mask = ~0;
	int carry = 0; //at the begin of addition
	int BLength = b.GetLength();
	int ALength = a.GetLength();
	int length, LessLength; 
	if (ALength <= BLength) {
		length = ALength;
		LessLength = BLength;
	}
	else {
		length = BLength;
		LessLength = ALength;
	}
	
	if(length+1 > result.GetCapacity()) {
		result.Resize(length + 1);
	}

	result.ar = result.al;

	int i;
	for(i=0; i<LessLength; i++) {
		glass = b.al[i] + a.al[i] + carry;
		result.al[i] = glass % mask;
		result.ar++;
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
	cout << result;
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
						} else if(tmp >= 0xA && tmp <= 0xF) {
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

		for(int k=0; k<n; k++){
				tmp_0 = 0;
				tmp_1 = 0;
				for(int i=0; i < block; i++) {
						index = length_s - k*block - i - 1;
						if(index < 2) //control start of the string 
								break;
						char symbol = string_for_num[index]; 
						if(symbol >= '0' && symbol <= '9') {
								tmp_1 = symbol - 48;
						} else if(symbol >= 'a' && symbol <= 'f') {
								tmp_1 = symbol - 87; //10 + symbol - 97;
								}
								else if(symbol >= 'A' && symbol <= 'F') {
									tmp_1 = symbol - 55; //symbol + 10 - 65
									} 
									else throw INCORRECT_SYMBOL;
						tmp_0 = tmp_0 | (tmp_1 << 4*i);
				}
				a.al[k] = tmp_0;
				a.ar++; //changing right range
		}
}
