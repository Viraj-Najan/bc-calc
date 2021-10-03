#include "stack.h"

int infixEval(char*, int, int);
int precedence(char);
int unaryEvaluate(operandStack*, char, int);
int binaryEvaluate(operandStack*, char, int);