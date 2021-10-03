#include <stdlib.h>
#include <limits.h>
#include "stack.h"

/*
 * int init(stack*, int);
 * Initializes the stack variable
 * init() takes pointer to stack structure and length of the stack as input
 *
 * RETURN VALUES:
 * 	Returns 1 on successful initialization
 * 	Returns 0 if malloc() fails the memory initialization
 */

int operatorInit(operatorStack *ptr, int length) { // for operators
	ptr->arr = (char*) malloc(length * sizeof(char)); // allocate the memory for stack
	if (ptr->arr == NULL)
		return 0;
	ptr->size = length;
	ptr->top = -1;
	return 1;
}

int operandInit(operandStack *ptr, int length) { // for operands
	ptr->arr = (Num*) malloc(length * sizeof(Num)); // allocate the memory for stack
	if (ptr->arr == NULL)
		return 0;
	ptr->size = length;
	ptr->top = -1;
	return 1;
}

/*
 * void deleteStack(stack*):
 * Takes a pointer to stack structure as input
 * Frees the memory allocated by malloc
 */

void operatorDeleteStack(operatorStack *ptr) { // for operators
	free(ptr->arr);
	ptr->size = 0;
	ptr->top = -1;
	return;
}

void operandDeleteStack(operandStack *ptr) { // for operands
	free(ptr->arr);
	ptr->size = 0;
	ptr->top = -1;
	return;
}

/*
 * int push(stack*, data);
 * Receives pointer to stack and the data as input
 * Returns 0 if Stack is full (Stack Overflow)
 * Returns 1 if data is successfully pushed onto stack
 */

int operatorPush(operatorStack *ptr, char data) { // for operator
	if (ptr->top == ptr->size - 1) // if stack is full
		return 0;
	(ptr->top)++;
	ptr->arr[ptr->top] = data;
	return 1;
}

int operandPush(operandStack *ptr, Num data) { // for operand
	if (ptr->top == ptr->size - 1) // if stack is full
		return 0;
	(ptr->top)++;
	ptr->arr[ptr->top] = data;
	return 1;
}

/*
 * char peek(stack*):
 * Receives pointer to stack as input
 * RETURN VALUES :
 * 	Returns '\0' if stack is empty (Stack Underflow)
 * 	Returns The value which is on the top of the stack
 */

char operatorPeek(operatorStack *ptr) { // for operator
	if (ptr->top == -1)
		return '\0';

	return ptr->arr[ptr->top];
}

Num operandPeek(operandStack *ptr) { // for operand
	// TAKE CARE OF STACK BEING EMPTY IN MAIN PROGRAM
	return ptr->arr[ptr->top];
}

/*
 * char pop(stack*) 
 * Pops the value from top of the stack
 * Pop means it deletes it from the stack and returns the topmost value
 * RETURN VALUES:
 * 	Returns '\0' when stack is empty
 * 	Returns The data on top of stack otherwise
 */

char operatorPop(operatorStack *ptr) { // for operator
	if (ptr->top == -1)
		return '\0';

	char d;
	d = ptr->arr[ptr->top];
	(ptr->top)--;
	return d;
}

Num operandPop(operandStack *ptr) { // for operand
	// TAKE CARE OF STACK BEING EMPTY IN MAIN PROGRAM
	Num d;
	d = ptr->arr[ptr->top];
	(ptr->top)--;
	return d;
}

int operatorIsFull(operatorStack *ptr) {
	if (ptr->top == ptr->size - 1)
		return 1;
	return 0;
}

int operandIsFull(operandStack *ptr) {
	if (ptr->top == ptr->size - 1)
		return 1;
	return 0;
}

int operatorIsEmpty(operatorStack *ptr) {
	if (ptr->top == -1)
		return 1;
	return 0;
}

int operandIsEmpty(operandStack *ptr) {
	if (ptr->top == -1)
		return 1;
	return 0;
}
