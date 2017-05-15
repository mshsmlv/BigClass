#include "big.h"
#include <stdlib.h>

using namespace BigErrors;
typedef unsigned int base;

Big :: Big() {
		al = new base[100];
		ah = al + 100 - 1;
		ar = al;
}

Big :: ~Big() {
		if(al) {
			delete [] al;
			al = NULL;
		}
}

Big :: Big(Big& old_n) {
		cout << "работает конструктор копирования" << endl;
		int capacity = old_n.ah - old_n.al + 1;
		int length = old_n.ar - old_n.al + 1;
		al = new base[capacity];
		ah = al + capacity - 1;
		ar = al + length - 1;
		for(int i = 0; i < length; i++) {
			al[i] = old_n.al[i];
		}
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
		return ar - al + 1;
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

Big Big :: Mul(base small) {
	Big result;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	
	if(result.GetCapacity() < this->GetLength() + 1) {
		result.Resize(GetLength()+1);
	}

	doubleBase cup;
	doubleBase carry = 0;
	result.ar=result.al;

	int i;
	for(i = 0; i < GetLength(); i++) {
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

Big Big :: Div(base small, base& remainder) {
	
	Big result;
	if(0 == small) {
		cout << "error" << endl;
	}


	result.Resize( GetCapacity() );

	doubleBase t = 0;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	remainder = 0; 
	

	result.ar = result.al + GetLength() - 1;
	for(int i = 0; i < result.GetLength(); i++) {
		result.al[i] = 0; 
	}


	//result.ar = result.al + result.GetCapacity();
	for(int i = GetLength() - 1; 0 <= i; i--) {
		t = static_cast<doubleBase>(al[i]) + 
				static_cast<doubleBase>(remainder) * mask;

	//	result.ar++;
		result.al[i] = static_cast<base>(t / small);
		remainder = static_cast<base>(t % small);
	}

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

Big Substraction(Big &b, Big &a) {
	int flag = Compare(b, a);
	Big alignment;
	if(-1 == flag) {
		alignment.Resize(a.GetCapacity());
		alignment.ar = alignment.al;
		int shift = a.GetLength();
		alignment.al[shift] = static_cast<base>(1);
		alignment.ar = alignment.al + shift;
		cout << "incompatible_operands" << endl;
		b = b + alignment; 
		cout << "alignment " << alignment;
		cout <<"before "<< b;
	}

	Big result;
	result = b - a;
	if(-1 == flag) {
		b = b - alignment;
		cout << "after " << b;
		cout << "Alength " << alignment.GetLength() << " blength " << b.GetLength() << endl;
	}
	return result;
}

Big& Big :: operator = (const Big &a) {
	if(GetCapacity() != a.GetCapacity()) {
		Resize(a.GetCapacity());
	}
	
	ar = al;
//cout << this -> GetLength() << endl;
	int length = a.GetLength();
	//cout << length << endl;
	
	for(int i=0; i<length; i++) {
		al[i] = a.al[i];
		ar++;
	}
	ar--;
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
		LessLength = ALength;
	}
	else {
		LessLength = BLength;
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
	for(i = 0; i<LessLength; i++) {
		glass = static_cast<doubleBase>(b.al[i]) + static_cast<doubleBase>(a.al[i]) + carry;
		result.al[i] = glass % mask;
		carry = !!(glass / mask); //for the next digit
		result.ar++;
	}
	//add tail from array, which longer
	if(i < ALength) {
		for(i = i; i < ALength; i++) {
			glass = a.al[i] + carry;
			result.al[i] = glass % mask;
			result.ar++;
			carry = !!(glass / mask);
			result.al[i] = a.al[i] + carry;
		}
	}

	else if(i < BLength){
		for(i = i; i < BLength; i++) { 
			glass = b.al[i] + carry;
			result.al[i] = glass % mask;
			result.ar++;
			carry = !!(glass / mask);
			result.al[i] = b.al[i] + carry;
		}
	}

	if (carry) {//digit carry in
			result.al[i] = carry;
			result.ar++;
	}
	result.ar--;
	result.Compress();
	cout << result;
	return result;
}


Big operator - (Big &b, Big &a) {
	int flag = Compare(b, a);
	if(-1 == flag) {
		cout << "incompatible_operands" << endl;
		throw INCOMPATIBLE_OPERANDS;
	}
	
	Big result;
	result.Resize(b.GetCapacity());
	result.ar = result.al;
	
	//if they are equal
	if(0 == flag) {
		result.al[0] = 0;
		return result;
	}

	int carry = 0;
	int given = 0;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8); //ЗДЕСЬ МАГИЯ!!!
	doubleBase cup = 0;
	doubleBase glass;

	int i;
	for(i = 0; i < a.GetLength(); i++) {

		result.ar++;
		cup = a.al[i] + carry;
		
		if(static_cast<doubleBase>(b.al[i]) < cup) {
			given = 1;
		}
		
		if(given) {
			glass  = static_cast<doubleBase>(b.al[i]) + 
						mask -
							cup; 
			result.al[i] = static_cast<base>(glass);
			carry = 1;
			given = 0;
		}

		else {
			result.al[i] = b.al[i] - static_cast<base>(cup);
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
	result.ar--;
	result.Compress();
	return result;
}

Big operator * (Big &b, Big &a) {
	
	Big result;
	if(a.GetLength() <= 1) {
		result = b.Mul(a.al[0]);
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

Big operator / (Big &b, Big &a) {
	Big result;
	if(a.GetLength() <= 1) {
		base remainder;
		result = b.Div(a.al[0], remainder);
		return result;
	}

	if(Compare(a, b) == 1) {
		result.al[0] = 0;
		return result;
	}

	doubleBase d, mask;
	mask = static_cast<doubleBase>(1) << (sizeof(base)*8); 	
	int order_digitA = a.GetLength();
	d = mask / static_cast<doubleBase>((a.al[order_digitA - 1]) + 1);

	//normalization

	//d is contains base
	b.Mul(static_cast<base>(d));
	a.Mul(static_cast<base>(d));

	//the starting initialization
	int n = a.GetLength();
	int m = b.GetLength() - n;
	int j = b.GetLength();
	doubleBase roof;
	doubleBase left_part, right_part;
	Big glass;
	Big new_num;
	Big q;
	q.Resize(b.GetCapacity());
	new_num.Resize(b.GetCapacity());

	for(j; 0 < j; j--) {
		roof = (static_cast<doubleBase>(b.al[j])*mask + static_cast<doubleBase>(b.al[j-1])) /
			static_cast<doubleBase>(a.al[n-1]);

		//checking the inequation
		while(1) {
			left_part = static_cast<doubleBase>(a.al[n-2]) * roof;
			right_part = (static_cast<doubleBase>(b.al[j]) * mask - 
					roof * static_cast<doubleBase>(a.al[n-1])) * mask +
				b.al[j-2];
			if(left_part > right_part) {
				roof--;
			}
			else {
				break;
			}
		}

		//imul and substraction
		glass = a.Mul(static_cast<base>(roof));
		new_num.ar = glass.ar;
		//формируем новое число для вычитания
		int l = 0;
		for(int k = j - n; k <= j; k++) {
			new_num.al[l] = a.al[k];
			new_num.ar++;		
			l++;
		}
		
		new_num = new_num - glass;
		cout << "lol" << endl;
		
		l = 0;
		for(int k = j - n; k <= j; k++) {
			a.al[k] = new_num.al[l];
			l++;
		}
		q.al[j] = static_cast<base>(roof);
	}
	cout << q << endl;
	return q;
}

ostream& operator << (ostream &out, Big &a) { 
	int block = sizeof(base)*2; // *8/4 how many numbers in the "base"
	int length = a.GetLength();
	int mask = 0xF;
	char tmp;
	unsigned int flag = 1;

	for(int i = a.GetLength() - 1; 0 <= i; i--) { //starting from the older
		for(int l = (block-1)*4; l >= 0; l-=4) {
			tmp = (a.al[i] & (mask << l)) >> l ; //get an each number from the block(one number - four bytes) 
			if(tmp >= 0 && tmp <= 9) {
				if(flag && 0 == tmp) {
					continue;
				}
				flag = 0;
				tmp = tmp + '0';
				out << tmp;
			} 
			else if(tmp >= 0xA && tmp <= 0xF) {
				tmp = tmp + 87;
				out << tmp;
			   	flag = 0;	
			}
			else cout << "error" << endl;
		}
	}

	if(flag) {
		cout << "0";
	}
	out << endl;
}

istream& operator >> (istream &in, Big &a) {
	int block = sizeof(base)*2; // *8 / 4
	base tmp_0, tmp_1; //tmp 0 | tmp_1 -> al[i]
	int index;
	string string_for_num;
//	cout << "Input num in 16:" << endl;
	in >> string_for_num;
	int length_s = string_for_num.length(); // includes "0x"
	int n = (length_s) / block + !!((length_s) % (block));
	if (n > a.GetCapacity()) { 
		a.Resize(n);
	} 
	for(int k=0; k<n; k++) {
		tmp_0 = 0;
		tmp_1 = 0;
		for(int i=0; i < block; i++) {
			index = length_s - k*block - i - 1;
			if(index < 0) //control start of the string 
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
	a.ar--;
}
