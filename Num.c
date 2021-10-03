#include "Num.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

void init(Num *n) { // initializes the Num
	n->head = NULL;
	n->tail = NULL;
	n->sign = 0;
	n->intCount = 0;
	n->decimalCount = 0;

	return;
}

int append(Num *n, char d) { // appends digit at the end of Num
	int i = 0;
	node *newNode;
	newNode = (node*) malloc(sizeof(node));
	if (newNode == NULL) // if malloc() fails
		return INT_MIN;
	newNode->prev = NULL;
	newNode->digit = d;
	newNode->next = NULL;

	if (n->head == NULL) { // if list is empty
		n->head = newNode;
		n->tail = newNode;
		return i;
	}
	n->tail->next = newNode;
	newNode->prev = n->tail;
	n->tail = newNode;
	
	return i + 1;
}

void length(Num *n) { // Calculates the intCount and decimalCount for the number
	node *p;
	p = n->head;
	int decimalFound = 0;
	n->intCount = 0;
	n->decimalCount = 0;

	while (p) {
		if (p->digit == '.') {
			decimalFound = 1;
			p = p->next;
			continue;
		}

		if (decimalFound)
			n->decimalCount += 1;
		else
			n->intCount += 1;
		p = p->next;
	}
	return;
}

void deleteNum(Num *n) { // frees the allotted memory to the Num
	if (n->head == NULL)
		return;
	node *q;
	do {
		q = n->head;
		n->head = n->head->next;
		free(q);
	} while (n->head != NULL);
	n->tail = NULL;

	n->intCount = 0;
	n->decimalCount = 0;
	n->sign = 0;
	return;
}

void print(Num *n) { // displays the Num
	if (n->head == NULL) {
		printf("0\n");
		return;
	}
	if (n->sign == 1)
		printf("-");
	node *p;
	p = n->head;
	while(p) {
		printf("%c", p->digit);
		p = p->next;
	}
	printf("\n");
	return;
}

void insertAtBeginning(Num *n, char d) { // inserts digit at the beginning. Useful for functions add(), sub(), mul()
	node *newNode;
	newNode = (node*) malloc(sizeof(node));
	if (newNode == NULL) // if malloc() fails
		return;
	
	newNode->digit = d;
	newNode->prev = NULL;

	if (n->head == NULL) { // if Num is empty
		newNode->next = NULL;
		n->head = n->tail = newNode;
		return;
	}
	
	newNode->next = n->head;
	n->head->prev = newNode;
	n->head = newNode;
	
	return;
}

/*
 * Num add(Num*, Num*)
 * adds to Nums and returns a Num
 */

Num add(Num *a, Num *b) { // a and b are pointers to the Num structure
	Num nadd; // The Num to be returned
	init(&nadd);

	if (a->sign == 1 && b->sign == 1) // both a and b are negative
		nadd.sign = 1;

	if (a->sign == 0 && b->sign == 1) { // a + (-b)
		b->sign = 0;
		return sub(a, b);
	}

	if (a->sign == 1 && b->sign == 0) { // -a + b 
		a->sign = 0;
		return sub(b, a);
	}

	int i, val1, val2;

	val1 = a->decimalCount;
	val2 = b->decimalCount;

	// append zeroes to the number which contains less number of decimal points
	
	if (val1 < val2) {
		if (val1 == 0) // a doesn't contain decimal numbers
			append(a, '.');
		for (i = 0; i < (val2 - val1); i++) {	
			append(a, '0');
			a->decimalCount += 1;
		}
	}

	else if (val1 > val2) {
		if (val2 == 0)
			append(b, '.');
		for (i = 0; i < (val1 - val2); i++) {
			append(b, '0');
			b->decimalCount += 1;
		}
	}

	val1 = a->intCount;
	val2 = b->intCount;

	// insert zeroes at beginning of the number which contains less intCount
	
	if (val1 < val2)
		for (i = 0; i < (val2 - val1); i++) {	
			insertAtBeginning(a, '0');
			a->intCount += 1;
		}
	else if (val1 > val2)
		for (i = 0; i < (val1 - val2); i++) {
			insertAtBeginning(b, '0');
			b->intCount += 1;
		}

	int digit1, digit2, carry = 0, sum, count; // these variables are for performing arithmetic
	node *ptra, *ptrb;
	ptra = a->tail; // ptra points to the tail of number a
	ptrb = b->tail; // ptrb points to the tail of number b

	if (a->decimalCount == 0) // both Num don't contain any decimal point
		count = a->intCount + a->decimalCount;
	else  // when decimal point is there we also need to consider the decimal point in iteration. So we add 1 to count
		count = a->intCount + a->decimalCount + 1;
	
	for (i = 0; i < count; i++) { 
		if (ptra->digit == '.') { // when decimal point occurs
			insertAtBeginning(&nadd, '.'); // put a decimal point in nadd also
			ptra = ptra->prev;
			ptrb = ptrb->prev;
			continue;
		}

		digit1 = ptra->digit - '0'; // extracting one digit from num1
		digit2 = ptrb->digit - '0'; // extracting one digit from num2
		sum = digit1 + digit2 + carry;
		if (sum >= 10) {
			carry = 1;
			sum = sum - 10;
		}
		else {
			carry = 0;
		}
		insertAtBeginning(&nadd, sum + '0'); // insert the digit obtained after doing sum in nadd
		ptra = ptra->prev; // decrement the pointers
		ptrb = ptrb->prev;
	}

	if (carry == 1) // if final carry generated is 1 
		insertAtBeginning(&nadd, '1');
		
	//deleteLeadingTrailingZeroes(&nadd);
	return nadd;
}

/*
 * compare(): Does Magnitude wise comparisons
 * returns 1 when a is greater than b
 * returns -1 when a is less than b
 * returns 0 when a is equal to b
 */

int compare(Num *a, Num *b) { // Doesn't consider the sign of the number
	if (a->intCount < b->intCount)
		return -1;

	if (a->intCount > b->intCount)
		return 1;

	node *ptra, *ptrb;

	ptra = a->head;
	ptrb = b->head;

	while (ptra && ptrb) {
		if (ptra->digit < ptrb->digit)
			return -1;
		else if (ptra->digit > ptrb->digit)
			return 1;
		ptra = ptra->next;
		ptrb = ptrb->next;
	}

	if (ptra != NULL) // if ptra has not ended it means that it is greater
		return 1;

	if (ptrb != NULL) // if ptrb has not ended
		return -1;

	//if (ptra == NULL && ptrb == NULL)
	return 0;
}

/*
 * Num sub(Num*, Num*)
 * subtracts two Nums and returns a Num
 */

Num sub(Num *a, Num *b) { // gives output Num as a - b
	Num *temp;
	Num nsub; // The Num to be returned
	init(&nsub);

	int cmp; 

	if (a->sign == 0 && b->sign == 0) { // both numbers are +ve
		cmp = compare(a, b);
		if (cmp == 0) // if two numbers are equal
			return nsub;

		if (cmp == -1) { // if a is less than b
			nsub.sign = 1; // make the subtracted number negative
			temp = a; // swap a and b
			a = b;
			b = temp;
		}
	} else if (a->sign == 0 && b->sign == 1) { // a is +ve and b is -ve. (a - (-b))
		b->sign = 0; // BEWARE THAT THE ORIGINAL SIGN OF THE NUMBER IS CHANGING HERE
		return add(a, b); // call add
	} else if (a->sign == 1 && b->sign == 0) { // -a - b
		b->sign = 1;
		return add(a, b);
	} else if (a->sign == 1 && b->sign == 1) { // -a - (-b) = b - a   Consider both b and a are +ve
		a->sign = 0;
		b->sign = 0;
		return sub(b, a);	
	}

	int i, val1, val2;

	val1 = a->decimalCount;
	val2 = b->decimalCount;

	// append zeroes to the number which contains less number of decimal points

	if (val1 < val2) {
		if (val1 == 0) // a doesn't contain decimal numbers
			append(a, '.');
		for (i = 0; i < (val2 - val1); i++) {	
			append(a, '0');
			a->decimalCount += 1;
		}
	}

	else if (val1 > val2) {
		if (val2 == 0)
			append(b, '.');
		for (i = 0; i < (val1 - val2); i++) {
			append(b, '0');
			b->decimalCount += 1;
		}
	}

	val1 = a->intCount;
	val2 = b->intCount;

	// insert zeroes at beginning to the number which contains less number of intCount

	if (val1 > val2)
		for (i = 0; i < (val1 - val2); i++) {
			insertAtBeginning(b, '0');
			b->intCount += 1;
		}

	int digit1, digit2, borrow = 0, diff, count; // these variables are for performing arithmetic
	node *ptra, *ptrb;
	ptra = a->tail; // ptra points to the tail of number a
	ptrb = b->tail; // ptrb points to the tail of number b

	if (a->decimalCount == 0) // both numbers don't contain decimal points
		count = a->intCount + a->decimalCount;
	else  // when decimal point is there we also need to consider the decimal point in iteration. So we add 1 to count
		count = a->intCount + a->decimalCount + 1;
	
	for (i = 0; i < count; i++) { 
		if (ptra->digit == '.') {
			insertAtBeginning(&nsub, '.');
			ptra = ptra->prev;
			ptrb = ptrb->prev;
			continue;
		}
		
		digit1 = ptra->digit - '0';
		digit2 = ptrb->digit - '0';
		diff = digit1 - digit2 - borrow;
		if (diff < 0) {
			borrow = 1;
			diff = diff + 10;
		}
		else {
			borrow = 0;
		}
		insertAtBeginning(&nsub, diff + '0');
		ptra = ptra->prev;
		ptrb = ptrb->prev;
	}		

	deleteLeadingTrailingZeroes(&nsub);
		
	return nsub;
}

Num mul(Num *a, Num *b) { // multiplies 2 Nums and returns a Num : O(n ^ 2)
	node *ptra, *ptrb;
	
	Num nmul; // Num to be returned
	init(&nmul);
	
	int i, j, mul, digit1, digit2, carry = 0;
	Num nAdd1, n2; // nAdd1 is the number which we will constantly add after each digit multiplication
	init(&nAdd1);
	init(&n2);

	ptrb = b->tail; // we begin with the last digit of b

	for (i = 0; i < b->intCount + b->decimalCount; i++) { // until Num b ends
		ptra = a->tail; // start with last digit of a

		digit1 = ptrb->digit - '0'; // extracting the digit
		for (j = 0; j < a->intCount + a->decimalCount; j++) { // until a ends
			digit2 = ptra->digit - '0';
			mul = (digit1 * digit2) + carry;
			insertAtBeginning(&nAdd1, (mul % 10) + '0');
			carry = mul / 10;
			ptra = ptra->prev; // decrementing ptra
			if (ptra && ptra->digit == '.') // ignore the decimal point
				ptra = ptra->prev;
		}
		insertAtBeginning(&nAdd1, carry + '0'); // carry generated at the end of first multiplication must be added
		carry = 0; // make carry 0 for the next multiplication
		for (j = 0; j < i; j++) // This loop is essential bcoz we add crosses when we multiply
			append(&nAdd1, '0');
		ptrb = ptrb->prev; // decrement ptrb by 1
		if (ptrb && ptrb->digit == '.') // ignore the decimal point
			ptrb = ptrb->prev;

		n2 = nmul; // for memory efficiency
		length(&nAdd1);
		length(&nmul);
		nmul = add(&nAdd1, &nmul); // add the nAdd1 generated
		deleteNum(&n2); // both n2 and nAdd1 are not needed
		deleteNum(&nAdd1);
	}
	if (a->sign == 1 && b->sign == 0) // -a * b
		nmul.sign = 1;
	else if (a->sign == 0 && b->sign == 1) // a * -b
		nmul.sign = 1;
	length(&nmul);
	if (a->decimalCount != 0 || b->decimalCount != 0) // finally insert the decimal point
		insertAt(&nmul, nmul.intCount - (a->decimalCount + b->decimalCount), '.');
	length(&nmul);
	deleteLeadingTrailingZeroes(&nmul);
	return nmul;
}

typedef struct Division { // Structure which contains both remainder and quotient
	Num quotient;
	Num remainder;
}Division;

/*
 * Algorithm for Division : https://youtu.be/6bpLYxk9TUQ
 */

Division division(Num *a, Num *b) { // Divides a by b and returns the Division Structure. The Quotient returned by this division() is an integer quotient. Decimal quotient is calculated by divi()
	a->sign = 0; // ignoring the signs of numbers
	b->sign = 0;

	int flag2 = 0; // For checking if numbers contain decimal points
	if (a->decimalCount || b->decimalCount)
		flag2 = 1;
	
	Division ret; // Structure to be returned

	Num quotient, nmul1, remainder, nadd1, n1, *currentQuotient, zero, acopy, bcopy; // acopy and bcopy are copies of a and b
	init(&quotient);
	init(&nmul1);
	init(&remainder);
	init(&nadd1);
	init(&n1);
	init(&zero);

	if (compare(a, b) == -1) { // if a is less than b
		ret.quotient = quotient; // quotient = 0
		Num zero; // zero is the value 0
		init(&zero);
		remainder = add(a, &zero); // add 0 to a to return 'a' itself as the remainder
		ret.remainder = remainder;
		return ret;	
	}

	acopy = add(a, &zero); // creating aCopy and bCopy by adding zero
	bcopy = add(b, &zero);
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	length(&acopy);
	length(&bcopy);
	deleteNum(&zero);

	if (a->decimalCount < b->decimalCount) { // shifting the decimal points in both the Numerator and denominator
		for (int i = 0; i < (b->decimalCount - a->decimalCount); i++)
			append(a, '0');
	} else if (a->decimalCount > b->decimalCount) {
		for (int i = 0; i < (a->decimalCount - b->decimalCount); i++)
			append(b, '0');
	}

	deleteNode(a, '.'); // deleting the decimal points
	deleteNode(b, '.');

	length(a);
	length(b);
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	
	int divisor, i, digit, carry, flag = 0, count, cmpResult;
	node *ptra;
	
	divisor = b->head->digit - '0'; // considering only the Most Significant digit of b (divisor)
	
	currentQuotient = a; // initially quotient is a (dividend)
	
	while (1) {
		carry = 0; // make carry 0 for each iteration
		ptra = currentQuotient->head; // first digit of currentQuotient
		if ((ptra->digit - '0') < divisor) { // if first digit is less than divisor then consider the next digit also
			digit = (ptra->digit - '0') * 10 + (ptra->next->digit - '0');
			ptra = ptra->next; // increment ptra bcoz we have considered the next digit also
			count = currentQuotient->intCount - 1 - (b->intCount - 1); // to be iterated count times
			flag = 1; // set flag to 1
		} else { // if first digit is greater than divisor
			count = currentQuotient->intCount - (b->intCount - 1);
		}
	
		for (i = 0; i < count; i++) {
			if (!flag) { // if first digit was greater than divisor
				digit = ptra->digit - '0';
			} else { 
				flag = 0;
			}
			digit = carry * 10 + digit;
			append(&nadd1, (digit / divisor) + '0');
			carry = digit % divisor;	
			ptra = ptra->next;
		}
	
		length(&nadd1);
		length(&quotient);
		n1 = quotient;
		quotient = add(&nadd1, &quotient); // add the division generated to the quotient
		deleteLeadingTrailingZeroes(&quotient);
		length(&quotient);
		deleteNum(&n1);
		deleteNum(&nadd1);
	
		nmul1 = mul(b, &quotient); // multiply entire divisor by quotient
		deleteNum(&remainder);
		deleteLeadingTrailingZeroes(a);
		length(a);
		remainder = sub(a, &nmul1); // subtract a and nmul1 to close we are to finding the answer
		length(&remainder);
		if (remainder.sign == 1) // if remainder is negative then next time subtract nadd1 from quotient
			nadd1.sign = 1; // so set sign of nadd1 as -ve for next iteration
	
		deleteNum(&nmul1);
		currentQuotient = &remainder; // setting the remainder obtained as currentQuotient to repeat the interation
		cmpResult = compare(&remainder, b); // compare the remainder and b
		if (cmpResult == -1) // if our remainder is less than the whole divisor then we have got our answer
			break;		
	}

	if (remainder.sign == 1) { // if finally the remainder is negative it means we need to subtract 1 from the quotient
		Num one; // one means the Number 1
		init(&one);
		append(&one, '1');
		n1 = quotient;
		quotient = sub(&quotient, &one); // subtract 1 from quotient
		deleteNum(&n1);
		length(&quotient);
		nmul1 = mul(b, &quotient);
		deleteNum(&remainder);
		remainder = sub(a, &nmul1); // accordingly calculate the modified remainder also
		deleteNum(&nmul1);		
	} 

	if (flag2) { // if number contains decimal points then we need to change the remainder
		nmul1 = mul(&bcopy, &quotient);
		deleteNum(&remainder);
		remainder = sub(&acopy, &nmul1);
		deleteNum(&nmul1);
	}
	deleteNum(&acopy);
	deleteNum(&bcopy);

	ret.quotient = quotient;
	ret.remainder = remainder;
	return ret;
}

Num divi(Num *a, Num *b, int scale) { // this function calls division() and takes care of decimal points in the division
	int aSign, bSign;
	aSign = a->sign; // store the signs of a and b
	bSign = b->sign;

	Num bcopy;
	init(&bcopy);
	bcopy = add(b, &bcopy);
	Division ret1 = division(a, b), ret2; // call division()

	if (scale == 0) { // if scale is 0 then simply return whatever we hav got from division()
		deleteNum(&ret1.remainder);
		if (aSign == 1 && bSign == 0) { // -a / b
			ret1.quotient.sign = 1;
		} else if (aSign == 0 && bSign == 1) { // a / -b
			ret1.quotient.sign = 1;
		}

		return ret1.quotient;
	}

	if (ret1.remainder.head == NULL) { // if remainder is 0
		append(&ret1.quotient, '.');
		for (int i = 0; i < scale; i++) // just append zeroes to the quotient and return
			append(&ret1.quotient, '0');
		deleteNum(&ret1.remainder);
		if (aSign == 1 && bSign == 0) {
			ret1.quotient.sign = 1;
		} else if (aSign == 0 && bSign == 1) {
			ret1.quotient.sign = 1;
		}
		return ret1.quotient;
	}

	Num remainder, n1, ten;
	init(&ten); // Create a number having value 10
	append(&ten, '1');
	append(&ten, '0');
	length(&ten);
	init(&n1);
	remainder = ret1.remainder;
	remainder.sign = 0;
		
	for (int i = 0; i < scale; i++) { // go on multiplying the remainder by 10 for scale times
		n1 = remainder;
		length(&remainder);
		remainder = mul(&remainder, &ten);
		deleteNum(&n1);
	}

	b = &bcopy;
	length(&remainder);
	length(b);
	ret2 = division(&remainder, b); // take the division of modified remainder and divisor
	deleteNum(&ret2.remainder);
	int retSign = ret1.quotient.sign;
	ret1.quotient.sign = 0;

	length(&ret2.quotient);
	for (int i = 0; i < (scale - ret2.quotient.intCount); i++)
		insertAtBeginning(&ret2.quotient, '0');

	// just append the division of modified remainder and divisor to the integer quotient got initially
	
	insertAtBeginning(&ret2.quotient, '.');
	length(&ret2.quotient);
	length(&ret1.quotient);
	n1 = ret1.quotient;
	ret1.quotient = add(&ret1.quotient, &ret2.quotient);
	ret1.quotient.sign = retSign;
	deleteNum(&n1);
	deleteNum(&ret2.quotient);

	deleteNum(&remainder);

	if (aSign == 1 && bSign == 0) {
		ret1.quotient.sign = 1;
	} else if (aSign == 0 && bSign == 1) {
		ret1.quotient.sign = 1;
	}
	return ret1.quotient;
}

Num mod(Num *a, Num *b, int scale) { // calculate modulus. This function also calls division()
	int aSign, bSign;
	aSign = a->sign;
	bSign = b->sign;

	if (scale == 0) { // if scale is 0 then just return whatever has been receiverd from division()
		Division ret = division(a, b);
		deleteNum(&ret.quotient);
		if (aSign == 1 && bSign == 0) {
			ret.remainder.sign = 1;
		} else if (aSign == 1 && bSign == 1) {
			ret.remainder.sign = 1;
		}
		return ret.remainder;
	}

	Num modulo, divis, nmul;
	init(&modulo);
	init(&nmul);
	
	divis = divi(a, b, scale); // call the divi() with the scale
	length(b);
	length(&divis);
	nmul = mul(b, &divis);
	length(a);
	length(&nmul);
	modulo = sub(a, &nmul); // find the modulo

	deleteNum(&divis);
	deleteNum(&nmul);

	if (aSign == 1 && bSign == 0) {
		modulo.sign = 1;
	} else if (aSign == 1 && bSign == 1) {
		modulo.sign = 1;
	}
	
	return modulo;
}

double NumToDouble(Num a) { // helper function for powe, s, c, t. Converts Num to double
	node *p = NULL; 
	p = a.head;
	double num = 0;
	
	while (p && p->digit != '.') {
		num = num * 10 + (p->digit - '0');
		p = p->next;
	}

	int mul = 10;

	if (p && p->digit == '.')
		p = p->next;
	while (p) {
		num = num + (double) (p->digit - '0') / mul;
		mul = mul * 10;
		p = p->next;	
	}

	if (a.sign == 1)
		num = -num;

	return num;
}

/* power operator
 * O(logn)
 *
 * Algorithm : https://drive.google.com/drive/folders/1lyXIg1cO8l6JqohtkF_nhjrd60jiCjiw
 */

Num powe(Num a, Num b, int scale) { // handle the case of x == 0 && y <= 0 in expression evaluator
	Num power, n1;
	init(&power);
	append(&power, '1'); // set initial value of power as 0
	init(&n1);	

	double expo = NumToDouble(b);
	int exponent;

	if (expo == (int) expo) { // if we have purely integer exponent

	} else { // ignoring the decimal part of exponent and displaying a warning to user
		printf("Warning : Exponent contains decimal values. Considering exponent as the equivalent integer\n");
	}

	exponent = (int) expo;
		
	
	int i, sign = 0;
	
	if (exponent < 0) {
		sign = 1; // setting sign to 1 if exponent is negative
		exponent = -exponent;
	}

	for (i = 0; exponent; i++) {
		if (exponent % 2) { // if exponent % 2 is 1 then only include the mul in power
			n1 = power;
			length(&power);
			length(&a);
			power = mul(&power, &a);
			deleteNum(&n1);	
		}
		n1 = a;
		length(&a);
		a = mul(&a, &a); // multiply
		deleteNum(&n1);
		exponent = exponent / 2;
	}

	deleteNum(&a);

	if (sign) { // if exponent is negative then
		n1 = power;
		Num one;
		init(&one);
		append(&one, '1');
		length(&one);
		length(&power);
		power = divi(&one, &power, scale); // 1 / power
		deleteNum(&n1);
	}

	return power;
}

/*
 * Num sqroot()
 *
 * Newton's Method Algorithm
 *
 * Xn+1 = (1 / 2) * (Xn + number / Xn)   Where Xn is the initial guess of what the square root may be
 * Xn+2 = (1 / 2) * (Xn+1 + number / Xn+1)
 *
 * This is continued until Xr+1 == Xr
 * Because when Xr+1 = Xr = sqrt then :
 *
 * sqrt = (1 / 2) * (sqrt + number / sqrt)
 * sqrt = (number) ** (1 / 2)
 *
 * Equivalent Python Program : https://drive.google.com/drive/folders/1ltYP4uGCRk-LfEdViDfq-Sf7vDGfEPd-
 */ 

Num sqroot(Num *a, int scale) { // Newton's method
	if (a->decimalCount == 0 && scale == 0) // setting minimum scale for integer
		scale = 1;
	if (a->decimalCount != 0 && scale <= a->decimalCount) // setting minimum scale for decimal number
		scale = a->decimalCount + 1;
	Num squareRoot, guess, two, n1, n2, numberByGuess, sum, zero, acopy;
	init(&zero);
	init(&squareRoot);
	init(&guess);
	init(&numberByGuess);
	init(&sum);
	init(&two);
	append(&two, '2');
	length(&two);
	acopy = add(a, &zero);

	if (a->head == NULL)
		return squareRoot;
	
	guess = divi(a, &two, scale); // setting guess = (a / 2)
	deleteNum(a);
	length(&acopy);
	*a = add(&acopy, &zero);

	while (1) {
		length(&guess);
		length(a);
		n1 = add(a, &zero);
		n2 = add(&guess, &zero);
		deleteNum(&numberByGuess);
		length(a);
		length(&guess);
		deleteLeadingTrailingZeroes(a);
		deleteLeadingTrailingZeroes(&guess);
		numberByGuess = divi(a, &guess, scale); // finding number / guess
		deleteNum(a);
		*a = add(&acopy, &zero);
		length(&numberByGuess);
		a = &n1;
		length(a);
		guess = n2;
		length(&guess);
		deleteNum(&sum);
		sum = add(&numberByGuess, &guess); // (number / guess) + guess
		length(&sum);
		deleteNum(&squareRoot);
		deleteNum(&two);
		append(&two, '2');
		length(&two);
		squareRoot = divi(&sum, &two, scale); // ((number / guess) + guess) / 2
		length(&guess);
		length(&squareRoot);
		deleteLeadingTrailingZeroes(&guess);
		deleteLeadingTrailingZeroes(&squareRoot);
		if (compare(&guess, &squareRoot) == 0) // when guess and square Root become same then break
			break;
		guess = squareRoot;
		deleteNum(a);
		*a = add(&acopy, &zero);
	}
		
	deleteNum(a);
	deleteNum(&guess);
	deleteNum(&numberByGuess);
	deleteNum(&sum);

	return squareRoot;
}

/*
 * sine function
 * Input angle must in Degrees
 * As value of sine is always in between -1 to 1 irrespective of how big the number is 
 * and as sine is a periodic function : We take mod of the number n with 360 and find the sine of that reduced number
 */

double s(Num n) {
	Num nReduced, threeSixty, nret;
	init(&nReduced);
	init(&threeSixty);
	init(&nret);
	append(&threeSixty, '3');
	append(&threeSixty, '6');
	append(&threeSixty, '0');
	length(&threeSixty);
	nReduced = mod(&n, &threeSixty, 0);
	deleteNum(&threeSixty);

	const double PI = (double) 22 / 7;
	double x, term, sum = 0;
	int i;

	x = NumToDouble(nReduced);
	deleteNum(&nReduced);

	if (x == 0 || x == 180)
		return 0;
	else if (x == 90)
		return 1;
	else if (x == 270)
		return -1;
	
	x = (x * PI) / 180; // converting degrees to radians
	
	term = x; // Initial term in sine series is x
	
	// finding sine using taylor series expansion

	for (i = 0; term; i++) { // stop the loop when term becomes 0
		if (i % 2 == 0)
			sum = sum + term;
		else
			sum = sum - term;

		term = (term * x * x) / ((2 * i + 1 + 1) * (2 * i + 1 + 2));  // next term in the series
	}

	return sum;
}

double c(Num n) { // input angle is in degrees
	Num nReduced, threeSixty, nret;
	init(&nReduced);
	init(&threeSixty);
	init(&nret);
	append(&threeSixty, '3');
	append(&threeSixty, '6');
	append(&threeSixty, '0');
	length(&threeSixty);
	nReduced = mod(&n, &threeSixty, 0);
	deleteNum(&threeSixty);

	const double PI = (double) 22 / 7;
	double x, term, sum = 0;
	int i;

	x = NumToDouble(nReduced);
	deleteNum(&nReduced);

	if (x == 90 || x == 270)
		return 0;
	else if (x == 0)
		return 1;
	else if (x == 180)
		return -1;
	
	x = (x * PI) / 180; // converting degrees to radians
	
	term = 1; // Initial term in cosine series is 1
	
	for (i = 0; term; i++) { // stop the loop when term becomes 0
		if (i % 2 == 0)
			sum = sum + term;
		else
			sum = sum - term;

		term = (term * x * x) / ((2 * i + 1) * (2 * i + 2));  // next term in the series
	}

	return sum;
}

double t(Num n) { // s() / c()
	Num nReduced, threeSixty, nret;
	init(&nReduced);
	init(&threeSixty);
	init(&nret);
	append(&threeSixty, '3');
	append(&threeSixty, '6');
	append(&threeSixty, '0');
	length(&threeSixty);
	nReduced = mod(&n, &threeSixty, 0);
	deleteNum(&threeSixty);

	double x;

	x = NumToDouble(nReduced);
	deleteNum(&nReduced);

	if (x == 90 || x == 270)
		return NAN;
	
	return s(n) / c(n);
}

int lessThan(Num *a, Num *b) { // <
	if (a->head == NULL && b->head == NULL) // a = 0 and b = 0
		return 0;
	if (a->sign == 1 && b->sign == 0) // a is -ve and b is +ve
		return 1;
	if (a->sign == 0 && b->sign == 1) // a is +ve and b is -ve
		return 0;
	int res = compare(a, b);
	if (res == 1 || res == 0)
		return 0;
	return 1;
}

int greaterThan(Num *a, Num *b) { // >
	if (a->head == NULL && b->head == NULL)
		return 0;
	if (a->sign == 0 && b->sign == 1) // 
		return 1;
	if (a->sign == 1 && b->sign == 0)
		return 0;
	int res = compare(a, b);
	if (res == -1 || res == 0)
		return 0;
	return 1;
}

int equalTo(Num *a, Num *b) { // ==
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	if (a->head == NULL && b->head == NULL)
		return 1;
	if (a->sign == 0 && b->sign == 1) // 
		return 0;
	if (a->sign == 1 && b->sign == 0)
		return 0;
	int res = compare(a, b);
	if (res == -1 || res == 1)
		return 0;
	return 1;
}

int notEqualTo(Num *a, Num *b) { // !=
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	if (a->head == NULL && b->head == NULL)
		return 0;
	if (a->sign == 0 && b->sign == 1) // 
		return 1;
	if (a->sign == 1 && b->sign == 0)
		return 1;
	int res = compare(a, b);
	if (res == -1 || res == 1)
		return 1;
	return 0;
}

int lessThanEqualTo(Num *a, Num *b) { // <=
	if (a->head == NULL && b->head == NULL)
		return 1;
	if (a->sign == 1 && b->sign == 0) // 
		return 1;
	if (a->sign == 0 && b->sign == 1)
		return 0;
	int res = compare(a, b);
	if (res == 1)
		return 0;
	return 1;
}

int greaterThanEqualTo(Num *a, Num *b) { // >=
	if (a->head == NULL && b->head == NULL)
		return 1;
	if (a->sign == 1 && b->sign == 0) // 
		return 0;
	if (a->sign == 0 && b->sign == 1)
		return 1;
	int res = compare(a, b);
	if (res == 1 || res == 0) 
		return 1;
	return 0;
}

int logicalAnd(Num *a, Num *b) { // &&
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	if (a->head == NULL || b->head == NULL) // returns false if any one of numbers os 0
		return 0;
	return 1;	
}

int logicalOr(Num *a, Num *b) { // ||
	deleteLeadingTrailingZeroes(a);
	deleteLeadingTrailingZeroes(b);
	if (a->head == NULL && b->head == NULL) // returns false if both numbers are 0
		return 0;
	return 1;	
}

int logicalNot(Num *a) {
	deleteLeadingTrailingZeroes(a);
	if (a->head == NULL)
		return 1;
	return 0;
}

char pop(Num *n) { // deletes and returns last digit from Num
	if (n->head == NULL) // if list is empty
		return '\0';
	int d;
	node *p, *q;
	p = n->head;
	if (p->next == NULL) { // if only one element is present in the list
		d = p->digit;
		free(p);
		n->head = NULL;
		n->tail = NULL;
		return d;
	}

	q = n->tail;
	d = q->digit;
	n->tail = q->prev;
	n->tail->next = NULL;
	free(q);
	return d;
}

char popFirst(Num *n) { // deletes and returns the first digit of Num
	if (n->head == NULL) // if list is empty
		return '\0';
	char d;
	node *p;
	p = n->head;
	if (p->next == NULL) { // if there is only one element in the list
		n->head = NULL;
		n->tail = NULL;
		d = p->digit;
		free(p);
		return d;
	}

	d = p->digit;
	n->head = p->next;
	n->head->prev = NULL;
	free(p);
	return d;

}

void deleteLeadingTrailingZeroes(Num *n) { // deletes the Leading and Trailing Unnecessary numbers from the number
	
	while (n->head && n->head->digit == '0') {
		n->intCount -= 1;	
		popFirst(n);
	}

	if (n->decimalCount == 0)
		return;
	
	while (n->tail->digit == '0') {
		n->decimalCount -= 1;
		pop(n);
	}

	if (n->decimalCount == 0)
		pop(n); // to remove decimal point

	return;
}

void insertAt(Num *n, int index, char element) {
	if (index == 0)
		return insertAtBeginning(n, element);

	if (index >= n->intCount + n->decimalCount) {
		append(n, element);
		return;
	}

	node *p, *newNode;
	newNode = (node*) malloc(sizeof(node));
	newNode->digit = element;
	p = n->head;
	int i;

	for (i = 0; i < index - 1; i++)
		p = p->next;

	p->next->prev = newNode;
	newNode->next = p->next;
	p->next = newNode;
	newNode->prev = p;

	return;
}

void deleteNode(Num *n, char element) {
	if (n->head == NULL)
		return;

	node *p;

	if (n->head->digit == '.') {
		p = n->head;
		n->head = n->head->next;
		n->head->prev = NULL;
		free(p);
		return;
	}

	if (n->tail->digit == '.') {
		p = n->tail;
		n->tail = n->tail->prev;
		n->tail->next = NULL;
		free(p);
		return;
	}

	p = n->tail;
	while (p && p->digit != '.')
		p = p->prev;
	
	if (p != NULL) {
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p);
	}

	return;
}
