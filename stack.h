#include "Num.h"

typedef struct operatorStack {
	char *arr;
	int size;
	int top;
}operatorStack;

typedef struct operandStack {
	Num *arr;
	int size;
	int top;
}operandStack;

int operatorInit(operatorStack*, int);
void operatorDeleteStack(operatorStack*);
int operatorPush(operatorStack*, char);
char operatorPop(operatorStack*);
char operatorPeek(operatorStack*);
int operatorIsFull(operatorStack*);
int operatorIsEmpty(operatorStack*);

int operandInit(operandStack*, int);
void operandDeleteStack(operandStack*);
int operandPush(operandStack*, Num);
Num operandPop(operandStack*);
Num operandPeek(operandStack*);
int operandIsFull(operandStack*);
int operandIsEmpty(operandStack*);