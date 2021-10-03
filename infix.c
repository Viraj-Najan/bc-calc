#include "infix.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

int precedence(char operator) {
	if (operator == 's' || operator == 'q' || operator == 'c' || operator == 't' || operator == 'u') // sin, sqrt, cos, tan, unary minus respectively
		return 50;
	if (operator == '^') // power operator
		return 40;
	if (operator == '*' || operator == '/' || operator == '%') // arithmetic
		return 30; 
	if (operator == '+' || operator == '-') // arithmetic
		return 20;
	if (operator == '<' || operator == '>' || operator == 'e' || operator == 'n' || operator == 'l' || operator == 'g') // <, >, ==, !=, <=, >= respectively
		return 15;
	if (operator == '!') // logical NOT
		return 14;
	if (operator == 'a') // logical AND
		return 13;
	if (operator == 'o') // logical OR
		return 12;
	if (operator == '(') // opening bracket lowest precedence
		return 10;
	return 0;
}

int unaryEvaluate(operandStack *s2, char ch, int scale) { // evaluates unary operators. Helper function for infixEval()
	if (s2->top >= 0) { // if operandStack contains at least one element then only unary operators can perform their function
		Num nPush, n1; // nPush is the Num to be pushed into stack after unary operation
		init(&nPush);
		double res; // for storing result of s(), c(), t()
		char str[20]; // for converting res into string
		int len, j; // len for calculating length of str
		n1 = operandPop(s2); // pop operand from operandStack and perform unary operation
		length(&n1); // find the intCount and decimalCount
		deleteLeadingTrailingZeroes(&n1); // delete all unnecessary zeroes
		switch (ch) {
			case 's': // sine
				res = s(n1);
				sprintf(str, "%lf", res); // convert the double to string
				len = strlen(str); 

				if (str[0] == '-') { // if value is negative
					nPush.sign = 1; // make the number to be pushed as negative
					j = 1; // to ignore the 0th element in str
				} else {
					j = 0; // consider the 0th element of str
				}
				for (; j < len; j++)
					append(&nPush, str[j]); // append all string chars in nPush

				length(&nPush); // find intCount and decimalCount of nPush
				operandPush(s2, nPush); // finally push
				deleteNum(&n1); // delete the popped operand
				break;

			case 'c': // cosine
				res = c(n1);
				sprintf(str, "%lf", res);
				len = strlen(str);

				if (str[0] == '-') {
					nPush.sign = 1;
					j = 1;
				} else {
					j = 0;
				}									
				for (; j < len; j++)
					append(&nPush, str[j]);

				length(&nPush);
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 't': // tangent
				res = t(n1);
				sprintf(str, "%lf", res);
				if (strcmp(str, "nan") == 0) { // t() returns nan when angle is 90 or 270. nan stands for not a number
					deleteNum(&n1);
					printf("tan(90) or tan(270) is undefined. ");	
					return INT_MIN;
				}
				len = strlen(str);

				if (str[0] == '-') {
					nPush.sign = 1;
					j = 1;
				} else {
					j = 0;
				}
				for (; j < len; j++)
					append(&nPush, str[j]);

				length(&nPush);
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 'q': // sqrt
				if (n1.sign == 1) {
					deleteNum(&n1);
					printf("Square Root of negative Number doesn't exist. ");	
					return INT_MIN;
				}

				nPush = sqroot(&n1, scale);
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 'u': // unary minus
				nPush = n1;
				if (n1.sign == 0) // if sign is 0 then make it 1 and vice versa
					nPush.sign = 1;
				else if (n1.sign == 1)
					nPush.sign = 0;
				operandPush(s2, nPush);
				break;

			case '!': // logical NOT
				append(&nPush, logicalNot(&n1) + '0');
				operandPush(s2, nPush);
				break;
		}
	} else { // of operandStack doesn't even contain 1 element
		printf("Too many operators and less operands. ");
		return INT_MIN;
	}

	return 1; // indicates success
}

int binaryEvaluate(operandStack *s2, char ch, int scale) { // evaluates binary operators. Helper function for infixEval()
	if (s2->top >= 1) { // if operandStack contains at least two elements then only binary operators can perform their function
		Num n1, n2, nPush; 
		init(&n1);
		init(&n2);
		init(&nPush); // nPush is to be pushed into stack after binary operator function
		n2 = operandPop(s2); // pop n2 from operandStack
		length(&n2);
		n1 = operandPop(s2); // pop n1 from stack
		length(&n1);
		deleteLeadingTrailingZeroes(&n1);
		deleteLeadingTrailingZeroes(&n2);
		switch (ch) {
			case '+':
				operandPush(s2, add(&n1, &n2));
				deleteNum(&n1); // delete n1 and n2 which were popped from the stack
				deleteNum(&n2);
				break;
			case '-':
				operandPush(s2, sub(&n1, &n2));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '*':
				operandPush(s2, mul(&n1, &n2));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '/':
				if (n2.head == NULL) { // if n2 is 0
					deleteNum(&n1);
					deleteNum(&n2);
					printf("DivisionByZero error. ");
					return INT_MIN;
				}
				operandPush(s2, divi(&n1, &n2, scale));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '%':
				if (n2.head == NULL) { // if n2 is 0
					deleteNum(&n1);
					deleteNum(&n2);
					printf("DivisionByZero error. ");
					return INT_MIN;
				}								
				operandPush(s2, mod(&n1, &n2, scale));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '^': // power
				if (n1.head == 0 && (n2.head == NULL || n2.sign == 1)) { // when n1 is 0 and n2 is <= 0
					deleteNum(&n1);
					deleteNum(&n2);
					printf("Base and Exponent cannot simultaneously be 0 and less than or equal to zero respectively. ");
					return INT_MIN;
				}

				if (n2.intCount > 10) { // if exponent is too large
					deleteNum(&n1);
					deleteNum(&n2);
					printf("Exponent too large. ");
					return INT_MIN;
				}
				operandPush(s2, powe(n1, n2, scale));
				deleteNum(&n2);
				break;

			case '<': // less than
				append(&nPush, lessThan(&n1, &n2) + '0'); // lessThan returns an int value so adding a '0' to it
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case '>': // greater than
				append(&nPush, greaterThan(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case 'e': // equal to (==)
				append(&nPush, equalTo(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;
 
			case 'n': // not equal to (!=)
				append(&nPush, notEqualTo(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case 'l': // <=
				append(&nPush, lessThanEqualTo(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case 'g': // >=
				append(&nPush, greaterThanEqualTo(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case 'o': // ||
				append(&nPush, logicalOr(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;

			case 'a': // &&
				append(&nPush, logicalAnd(&n1, &n2) + '0');
				operandPush(s2, nPush);
				deleteNum(&n2);
				deleteNum(&n1);
				break;
				
			default: // When operatorPop() returns 0 then it means operator stack is empty
				deleteNum(&n1);
				deleteNum(&n2);
				printf("Too many operands and less operators. ");
				return INT_MIN;
		}
	} else { // there are less operands and more operators
		printf("Too many operators and less operands. ");
		return INT_MIN;
	}

	return 1; // successful evaluation
}

/*
 * int infixEval(char*, int, int);
 * Evaluates the infix expression
 * Returns INT_MIN if expression is invalid
 */

int infixEval(char *str, int len, int scale) {
	if (str[0] < '0' || str[0] > '9') { // if 0th char of str is something different than a digit 
		if (str[0] == '(' || ((str[0] == '-' || str[0] == '!') && ((str[1] >= '0' && str[1] <= '9') || str[1] == '(' || str[1] == 's' || str[1] == 'c' || str[1] == 'q' || str[1] == 't' || str[1] == '-' || str[1] == '!'))) { // if bracket or unary minus or logical NOT is there in the beginning then allowed

		} else if (str[0] == 's' || str[0] == 'c' || str[0] == 't') { // if sqrt, sin, cos, tan is there in beginning then allowed

		} else { // anything else if not allowed
			printf("Expression should start either with a digit or unary operator. ");		
			return INT_MIN;
		}
	}
	
	operatorStack s1; // for storing operators
	operandStack s2; // for storing operands
	operatorInit(&s1, len); // initializing the size as length of the input string
	operandInit(&s2, len);

	Num n, ndel; // n is used for pushing numbers into operandStack and ndel is used to delete remaining numbers from operandStack
	init(&n);
	
	int i, flag1 = 0; // flag1 is for finding occurence of multiple decimal points
	char ch;
	
	for (i = 0; str[i]; i++) { // until '\0' of str[i] occurs
		if (str[i] >= '0' && str[i] <= '9') { // if digit is found
			append(&n, str[i]); // append it to Num n		
			if (str[i + 1] == '.' || (str[i + 1] >= '0' && str[i + 1] <= '9')) { // if next char is a decimal point or digit
				do {
					i++;
					if (flag1 == 0 && str[i] == '.') { // for validating number of decimal points in a single number
						flag1 = 1;
					} else if (flag1 == 1 && str[i] == '.') { // if more than one decimal point is there in one number
						operatorDeleteStack(&s1); // delete operatorStack
						while (!operandIsEmpty(&s2)) { // delete all Num in operandStack
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2); // finally delete operandStack
						printf("Too many decimal points in a single number. ");
						return INT_MIN;
					}
					append(&n, str[i]); // append it to Num n
				} while (str[i + 1] == '.' || (str[i + 1] >= '0' && str[i + 1] <= '9')); // until any non digit or non decimal point char is found
			} 

			deleteLeadingTrailingZeroes(&n);
			operandPush(&s2, n); // push the Num n into s2
			
			if (str[i + 1] == ' ' || str[i + 1] == ')' || str[i + 1] == '\0' || str[i + 1] == '\t') { // if space, ')', '\0' or tab is encountered after a number then only it is a valid number
				flag1 = 0; // set the decimal point flag to zero
				n.head = n.tail = NULL; // make n ready for push next operand
				n.sign = n.intCount = n.decimalCount = 0;
			} else { // if space or tab is not there after number
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2); 
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Space or tab is mandatory after every operand. ");
				return INT_MIN;
			}

		} else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%' || str[i] == '<' || str[i] == '>' || (str[i] == '=' && str[i + 1] == '=') || (str[i] == '!' && str[i + 1] == '=') || (str[i] == '&' && str[i + 1] == '&') || (str[i] == '|' && str[i + 1] == '|')) { // if operators are found

			if ((str[i] == '!' && str[i + 1] == '=') || (str[i] == '=' && str[i + 1] == '=') || (str[i] == '<' && str[i + 1] == '=') || (str[i] == '>' && str[i + 1] == '=') || (str[i] == '&' && str[i + 1] == '&') || (str[i] == '|' && str[i + 1] == '|')) // these operators contain 2 characters
				i++; // so increment i by 1

			if (str[i] == '-' && (str[i + 1] >= '0' && str[i + 1] <= '9')) { // if - is unary minus
				operatorPush(&s1, 'u');	
			} else if (str[i] == '-' && str[i + 1] == '(') { // minus followed by an opening bracket is also a case of unary minus
				operatorPush(&s1, 'u');
			} else if (str[i] == '-' && (str[i + 1] == 's' || str[i + 1] == 'c' || str[i + 1] == 't')) { // - followed by s(), c() or t()
				operatorPush(&s1, 'u');	
			} else if (str[i] == '-' && str[i + 1] == '!') { // - followed by !
				operatorPush(&s1, 'u');
			} else { // if operator is not unary minus
				if (str[i + 1] == ' ' || str[i + 1] == '\t') { // if there is a space after operator
					while (!operatorIsEmpty(&s1) && precedence(str[i]) <= precedence(operatorPeek(&s1))) { // until stack becomes empty and until we find operators having higher or equal precedence in the stack
						ch = operatorPop(&s1); // the binary operator
						
						if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u' || ch == '!') { // for unary operator
							if (unaryEvaluate(&s2, ch, scale) == INT_MIN) { // call the unaryEvaluate() function
								operatorDeleteStack(&s1);
								while (!operandIsEmpty(&s2)) {
									ndel = operandPop(&s2);
									deleteNum(&ndel);
								}
								operandDeleteStack(&s2);
								return INT_MIN;
							}
						} else { // for binary operator
							if (binaryEvaluate(&s2, ch, scale) == INT_MIN) { // call the binaryEvaluate() function
								operatorDeleteStack(&s1);
								while (!operandIsEmpty(&s2)) {
									ndel = operandPop(&s2);
									deleteNum(&ndel);
								}
								operandDeleteStack(&s2);
								return INT_MIN;
							}
						}	
					}
					if (str[i - 1] == '=' && str[i] == '=') { // ==
						operatorPush(&s1, 'e');
					} else if (str[i - 1] == '!' && str[i] == '=') { // !=
						operatorPush(&s1, 'n');
					} else if (str[i - 1] == '<' && str[i] == '=') { // <=
						operatorPush(&s1, 'l');
					} else if (str[i - 1] == '>' && str[i] == '=') { // >=
						operatorPush(&s1, 'g');
					} else if (str[i - 1] == '&' && str[i] == '&') { // &&
						operatorPush(&s1, 'a');
					} else if (str[i - 1] == '|' && str[i] == '|') { // ||
						operatorPush(&s1, 'o');
					} else { // when operator spans a single character, eg +, -, /, *, >, <
						operatorPush(&s1, str[i]); 
					}
				} else { // if there is no space or tab after operator
					operatorDeleteStack(&s1);
					while (!operandIsEmpty(&s2)) {
						ndel = operandPop(&s2);
						deleteNum(&ndel);
					}
					operandDeleteStack(&s2);
					printf("Every operator (except unary) must be followed by a space or tab. ");
					return INT_MIN;
				}
			}
	
		} else if (str[i] == '^') { // if we come across exponential operator
			if (str[i + 1] == ' ' || str[i + 1] == '\t') { // if space is there after exponential operator
				while (!operatorIsEmpty(&s1) && precedence(str[i]) < precedence(operatorPeek(&s1))) { // pop until stack is empty and higher precedence operator is there in stack
					// here we don't conside higher AND EQUAL precedence while popping bcoz ^ is right associative
					ch = operatorPop(&s1); 
					
					if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u' || ch == '!') { // for unary operator
						if (unaryEvaluate(&s2, ch, scale) == INT_MIN) { // call unaryEvaluate
							operatorDeleteStack(&s1);
							while (!operandIsEmpty(&s2)) {
								ndel = operandPop(&s2);
								deleteNum(&ndel);
							}
							operandDeleteStack(&s2);
							return INT_MIN;
						}
					}
					// no need to call binary evaluate bcoz only unary operators have higher precedence than ^
				}
				operatorPush(&s1, '^');
			} else { // if space is not there after ^
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Every operator (except unary) must be followed by a space or tab. ");
				return INT_MIN;
			} 
		} else if (str[i] == '(') { // if '(' is found then simply push to stack
			operatorPush(&s1, '(');
		} else if (str[i] == ')') {
			while (!operatorIsEmpty(&s1) && (ch = operatorPop(&s1)) != '(') { // pop until stack is empty and we find a '('
				if (operatorIsEmpty(&s1) && ch != '(') { // if ')' is present but '(' is not present
					operatorDeleteStack(&s1);
					while (!operandIsEmpty(&s2)) {
						ndel = operandPop(&s2);
						deleteNum(&ndel);
					}
					operandDeleteStack(&s2);
					printf("( is missing. ");
					return INT_MIN;
				}
				
				if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u' || ch == '!') { // unary operators
					if (unaryEvaluate(&s2, ch, scale) == INT_MIN) { // call unaryEvaluate()
						operatorDeleteStack(&s1);
						while (!operandIsEmpty(&s2)) {
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2);
						return INT_MIN;
					}
				} else { // call binaryEvaluate()
					if (binaryEvaluate(&s2, ch, scale) == INT_MIN) {
						operatorDeleteStack(&s1);
						while (!operandIsEmpty(&s2)) {
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2);
						return INT_MIN;
					}
				}
			}

		} else if (str[i] == ' ' || str[i] == '\t') { // space or tab
			// do nothing
		} else if (str[i] == 's') { // operator can be sqrt or s
			if (str[i + 1] == 'q' && str[i + 2] == 'r' && str[i + 3] == 't' && str[i + 4] == '(') { // sqrt
				operatorPush(&s1, 'q');
				i = i + 3; // because sqrt contains 4 chars
			} else if (str[i + 1] == '(') { // s
				operatorPush(&s1, 's');
			} else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else if (str[i] == 'c') { // cosine
			if (str[i + 1] == '(')
				operatorPush(&s1, 'c');
			else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else if (str[i] == 't') { // tangent
			if (str[i + 1] == '(')
				operatorPush(&s1, 't');	
			else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else if (str[i] == '!') { // if logical not comes then simply push in the stack doing nothing because ! is non associative
			operatorPush(&s1, '!');
		} else {
			operatorDeleteStack(&s1);
			while (!operandIsEmpty(&s2)) {
				ndel = operandPop(&s2);
				deleteNum(&ndel);
			}
			operandDeleteStack(&s2);
			printf("Invalid Character. ");
			return INT_MIN;
		}
	}

	// str[i] has reached the end now

	do { // pop the remaining operators from the stack
		ch = operatorPop(&s1);
		
		if (ch == '(') { // if '(' is found here, this means that there was a missing ')'
			operatorDeleteStack(&s1);
			while (!operandIsEmpty(&s2)) {
				ndel = operandPop(&s2);
				deleteNum(&ndel);
			}
			operandDeleteStack(&s2);
			printf(") is missing. ");
			return INT_MIN;
		}

		if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u' || ch == '!') { // unary operator
			if (unaryEvaluate(&s2, ch, scale) == INT_MIN) {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				return INT_MIN;
			}
		} else {
			if (s2.top == 0 && ch == '\0') { // operandStack has only one element and operatorStack is empty means expression eval is done perfectly
				// do nothing
			} else if (binaryEvaluate(&s2, ch, scale) == INT_MIN) {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				return INT_MIN;
			}		
		}
	} while (ch != '\0'); // operatorPop() returns '\0' when stack becomes empty

	ndel = operandPop(&s2); // final value of expression 
	print(&ndel);
	deleteNum(&ndel);

	operatorDeleteStack(&s1);
	while (!operandIsEmpty(&s2)) {
		ndel = operandPop(&s2);
		deleteNum(&ndel);
	}
	operandDeleteStack(&s2);
	return 1; // indicates success
}
