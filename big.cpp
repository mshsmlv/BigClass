#include "big.h"
#include <stdlib.h>

using namespace BigErrors;
const int DEBUG_MODE = 0;

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
	//	cout << "работает конструктор копирования" << endl;
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
	ar--;
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
		if(GetCapacity() < new_capacity) {
				if(al) 
					delete [] al;
				al = new base[new_capacity];
				ah = al + new_capacity - 1;
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

	if(0 == small) {
		result.al[0] = 0;
		result.ar = result.al;
		return result;
	}
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	
	if(result.GetCapacity() < GetLength() + 1) {
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
	result.Compress();
	return result;
}

Big Big :: Div(base small, base& remainder) {
	
	Big result;
	if(0 == small) {
		throw DIV_ZERO;
	}

	if(GetLength() <= 1 && 0 == al[0]) { //comparison with zero
		result.ar = result.al;
		result.al[0] = 0;
		remainder = small;
		return result;
	}


	result.Resize( GetCapacity() );

	doubleBase t = 0;
	doubleBase mask = static_cast<doubleBase>(1) << (sizeof(base)*8);
	remainder = 0; 
	

	result.ar = result.al + GetLength() - 1;
	for(int i = 0; i < result.GetLength(); i++) {
		result.al[i] = 0; 
	}


	for(int i = GetLength() - 1; 0 <= i; i--) {
		t = static_cast<doubleBase>(al[i]) + 
				static_cast<doubleBase>(remainder) * mask;

		result.al[i] = static_cast<base>(t / small);
		remainder = static_cast<base>(t % static_cast<doubleBase>(small));
	}

	result.Compress();
	return result;
}

int Compare(const Big &b, const Big &a) {
	if(b.GetLength() > a.GetLength()) {
		return 1;
	}
	else if(b.GetLength() < a.GetLength()) {
		//	cout << "b = " << b.GetLength() << endl;

		//	cout << "a = " << a.GetLength() << endl;

		//	cout << "ушел отсюда " << endl;
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


Big& Big :: operator = (const Big &a) {
	if(GetCapacity() != a.GetCapacity()) {
		Resize(a.GetCapacity());
	}
	
	ar = al;
	int length = a.GetLength();
	
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
	//cout << "BLENGTH = " << BLength << endl;
	//cout << "ALENGTH = " << ALength << endl;
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
	for(i = 0; i < LessLength; i++) {
		glass = static_cast<doubleBase>(b.al[i]) + static_cast<doubleBase>(a.al[i]) + carry;
		result.al[i] = glass % mask;
		//cout << "r.al[i] = " << hex << result.al[i] << endl;
		carry = !!(glass / mask); //for the next digit
		result.ar++;
	}
	//add tail from array, which longer
	if(i < ALength) {
		//cout << "ALENGTH" << endl;
		for(i = i; i < ALength; i++) {
			glass = a.al[i] + carry;
			result.al[i] = glass % mask;
			result.ar++;
			carry = !!(glass / mask);
		}
	}

	else if(i < BLength){
		//cout << "BLENGTH" << endl;
		for(i = i; i < BLength; i++) { 
			glass = b.al[i] + carry;
		//	cout << "glass = " << hex << glass << endl;
			result.al[i] = glass % mask;
		//	cout << "sdgsdfgdsg = " << hex << result.al[i] << endl;
			result.ar++;
			carry = !!(glass / mask);
		//cout << "result " << result << endl;
		}
	}

	if (carry) {//digit carry in
			result.al[i] = carry;
			result.ar++;
	}
	result.ar--;
	result.Compress();
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
			carry = 0;
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
	for(int i = 0; i<result.GetCapacity(); i++) {
		result.al[i] = 0;
		result.ar++;
	}
	result.ar--;
//cout << "result length from function = " << result.GetLength() << endl; 

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

Big Division(Big &e, Big &c, Big &remainder) {

	Big b;
	Big a;
	b.Resize(e.GetLength() + 1);

	b = e;
	a = c;

	if(DEBUG_MODE) {
		cout << "b = " << b << endl;
		cout << "c = " << c << endl;
		cout << "DELENIE" << endl;
	}
	Big result;

	if(-1 == Compare(b,c)) {
		result.ar = result.al;
		result.al[0] = 0;
		remainder = b;
		return result;
	}

	if(a.GetLength() <= 1) {
		base r;
		result = b.Div(a.al[0], r);
		remainder.al[0] = r;
		remainder.ar = remainder.al;
		return result;
	}

	if(Compare(a, b) == 1) {
		result.al[0] = 0;
		result.ar = result.al;
		remainder.al[0] = 0;
		remainder.ar = remainder.al;
		return result;
	}

	doubleBase d, mask;
	mask = static_cast<doubleBase>(1) << (sizeof(base)*8); 	
	int order_digitA = a.GetLength() - 1;
	d = mask / static_cast<doubleBase>((a.al[order_digitA]) + 1);

	if(DEBUG_MODE) {
		cout << "a.al[order]: " << hex << a.al[order_digitA] << endl;
		cout << "mask: " << hex << mask << endl;
		cout << "d = " << d << endl;
	}
	
	//normalization
	//d is contains base
	b.al[b.GetLength()] = 0;
	int j = b.GetLength();
	int n = a.GetLength();
	int m = b.GetLength() - n;
	int B_Length = b.GetLength();
	
	if(DEBUG_MODE) {
		cout << "n = " << n << " m = " << m << endl;
	}	
	
	b = b.Mul(static_cast<base>(d));
	a = a.Mul(static_cast<base>(d));

	if(DEBUG_MODE) {	
		cout << "NORMALIZATION:" << endl;
		cout << "b = " << b << endl;
		cout << "a = " << a << endl;
	}
	
	//the starting initialization
	doubleBase roof;
	doubleBase left_part, right_part;
	Big glass;
	Big new_num;
	Big q;
	q.Resize(b.GetCapacity());
	new_num.Resize(b.GetCapacity());
	q.ar = q.al + m + 1;
	for(int i = 0; i < q.GetLength(); i++) {
		q.al[i] = 0;
	}

	for(j; (B_Length - m) <= j; j--) {

		if(DEBUG_MODE) {
			cout << "=========================================" << endl;
			cout << "j = " << j << endl;
		}
		roof = (static_cast<doubleBase>(b.al[j])*mask + static_cast<doubleBase>(b.al[j-1])) /
			static_cast<doubleBase>(a.al[n-1]);

		if(DEBUG_MODE) {
			cout << "roof1 = " << hex << roof << endl;
			cout << "a.al[n-2]: " << hex << a.al[n-2] << endl;
			cout << "b.al[j]: " << hex << b.al[j] << endl;
			cout << "b.al[j-1]: " << hex << b.al[j-1] << endl;
			cout << "b.al[j-2]: " << hex << b.al[j-2] << endl;
		}
		//checking the inequation
		//здесь будем отслеживать переполнение правой части
		while(1) {
			left_part = static_cast<doubleBase>(a.al[n-2]) * roof;
			right_part = static_cast<doubleBase>(b.al[j]) * mask + 
							static_cast<doubleBase>(b.al[j-1]) -  roof * 
								static_cast<doubleBase>(a.al[n-1]);
			if(right_part & mask) {
				if(DEBUG_MODE) cout << "переполнение правой части " << endl;
				break; // дальнейшее умножение приведет к переполнению doubleBase
			}

			right_part = right_part * mask + b.al[j-2];
			if(DEBUG_MODE) {
				cout << "left_part:" << left_part << endl << "right_part:" << right_part << endl;
			}
			if(left_part > right_part) {
				roof--;
				if(DEBUG_MODE) cout << "roof--" << endl;
			}
			else {
				break;
			}
		}

		//imul and substraction
	if(DEBUG_MODE) 	cout << endl << "roof = " << roof << endl;
		glass = a.Mul(static_cast<base>(roof));
	if(DEBUG_MODE) 	cout << "glass = " << glass << endl;
		
		//формируем новое число для вычитания
		new_num.ar = new_num.al;
		int l = 0;
		base flag = 1;
		for(int k = j-n; b.al[k] | flag; k++) {
			if(0 != b.al[k]) {
				flag = 0;
			}
			if(DEBUG_MODE) {
				cout << "k=" << k << endl;
				cout << "формирует новое число" << endl;
			}
			new_num.al[l] = b.al[k];
			new_num.ar++;		
			l++;
		}

		int storaged_length_new_num = new_num.GetLength();

	if(DEBUG_MODE) 	cout << "new_num = " << new_num << endl;
		
		
		if(-1 == Compare(new_num, glass)) {
			roof--;
			if(DEBUG_MODE){
				cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
				cout << "roof = " << roof << endl;
			}
			glass = a.Mul(static_cast<base>(roof));
			if(DEBUG_MODE) {
				cout << "glass1 = " << glass << endl;
				cout << "new_num = " << new_num << endl;
				cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
			}
		}

		if(DEBUG_MODE)	cout << "glass = " << glass << endl;
		new_num = new_num - glass;


		int length_new_num = new_num.GetLength();
		while(length_new_num < storaged_length_new_num) {
			if(DEBUG_MODE)	cout << "добивание нулями" << endl;
			new_num.al[length_new_num] = 0;
			new_num.ar++;
			length_new_num++;
		}
		if(DEBUG_MODE) cout << "new_num = " << new_num << endl;
		
		l = 0;
		flag = 1;
		for(int k = j-n; b.al[k] | flag; k++) {
			if(0 != b.al[k]) {
				flag = 0;
			}
			b.al[k] = new_num.al[l];
			l++;
		}

		if(DEBUG_MODE) cout << "b = " << b << endl;
		q.al[j-n] = static_cast<base>(roof);
	}
	//DENORMALIZATION
	base r;
	b = b.Div(d, r);
	q.Compress();
	if(DEBUG_MODE) cout << "q = " << q << endl;
	remainder = b;
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
			else throw INCORRECT_SYMBOL;
		}
	}


	if(flag) {
		out << "0";
	}
//	out << endl;
	return out;
}

istream& operator >> (istream &in, Big &a) {
	int block = sizeof(base)*2; // *8 / 4
	base tmp_0, tmp_1; //tmp 0 | tmp_1 -> al[i]
	int index;
	string string_for_num;
	in >> string_for_num;
	int length_s = string_for_num.length(); 
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
	return in;
}
