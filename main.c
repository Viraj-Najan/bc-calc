#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "infix.h"

int readLine(char**); 

int main(int argc, char **argv) {

	if (argc == 2) {
		if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
			printf("usage : ./bc [options]\n");
			printf("  -h   --help      displays all flags and what those flags do\n");
			printf("  -v   --version   displays the version of bc\n");
			printf("  -i   --info      displays the general info about bc\n");
			printf("  -r   --rules     displays the rules while using bc\n");
			printf("  -e   --errors    displays the errors handled in bc\n");
			return 0;
		} 

		if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
			printf("bc version 1.0\n");
			return 0;
		}

		if (!strcmp(argv[1], "--info") || !strcmp(argv[1], "-i")) {
			printf("bc is an arbitrary precision calculator\n");
			printf("It evaluates expressions containing various operators according to predefined precedence.\n");
			printf("Operators : \n");
			printf("1) Brackets : ()\n");
			printf("2) Arithmetic Operators : +, -, *, /, %%\n");
			printf("3) Power Operator       : ^\n");
			printf("4) Relational Operators : <, >, <=, >=, ==, !=\n");
			printf("5) Logical Operators    : &&, ||, !\n");
			printf("6) Math Functions       : sqrt(), s(), c(), t()\n");
			printf("                                  sin, cos, tan\n\n\n");
			printf("Angle given to s(), c(), t() must be in degrees\n");
			printf("Type quit to exit out of the program\n\n");
			printf("Type clear to clear the terminal screen\n\n");
			printf("scale variable can be set to obtain the desired scale of division, modulo and power operator\n");
			printf("scale = value <- to set the scale variable\n");
			printf("scale <- to get the value of scale variable\n");
			return 0;
		}

		if (!strcmp(argv[1], "--rules") || !strcmp(argv[1], "-r")) {
			printf("Precedence Rules are as follows : (from highest to lowest)\n");
			printf("s(), c(), t(), sqrt(), unary -      non-associative\n");
			printf("^                                   right-associative\n");
			printf("*, /, %%                             left-associative\n");
			printf("+, -                                left-associative\n");
			printf("<, >, <=, >=, ==, !=                left-associative\n");
			printf("!                                   non-associative\n");
			printf("&&                                  left-associative\n");
			printf("||                                  left-associative\n\n\n");
			printf("Expression should start either with a digit or unary operator.\n");
			printf("Each Operator (except unary) and Operand must be followed by at least one space or one tab\n\n");
			printf("sqrt() will always return a positive value and not a +/- value.\n");
			return 0;	
		}

		if (!strcmp(argv[1], "--errors") || !strcmp(argv[1], "-e")) {
			printf("tan(90) or tan(270) is undefined.\n");
			printf("Square Root of negative Number doesn't exist.\n");
			printf("Too many operators and less operands. (eg 1 + + +)\n");
			printf("Too many operands and less operators. (eg 1 + 1 2)\n");	
			printf("DivisionByZero error. (eg. 1 / 0 or s(90) / c(90) or 1 %% 0)\n");	
			printf("Base and Exponent cannot simultaneously be 0 and less than or equal to zero respectively. (eg 0 ^ 0 or 0 ^ -2)\n");
			printf("Exponent too large. (if exponent in power has greater than 10 digits)\n");
			printf("Too many decimal points in a single number. (eg 1.2.3 + 2)\n");
			printf("( is missing. (eg 1 + 2) )\n");
			printf(") is missing. (eg (1 + 2 )\n");
			printf("Invalid Character. (eg a + b)\n");
			printf("Warning : Exponent contains decimal values. Considering exponent as the equivalent integer. (eg 23 ^ 1.36)\n");	
			return 0;
		} else {
			printf("Invalid Option. Try : ./bc --help\n");
			return 0;
		}	
	} else if (argc == 1) {

	} else {
		printf("usage : ./bc [options]\n");
		return 0;		
	}

	char *str = NULL;
	int length, scale = 0, i, flag = 0; // scale is the scale variable

	do {
		length = readLine(&str); // take input

		if (strstr(str, "scale")) { // strstr() returns NULL if substring is not found
			if (str[5] == '\0') { // if user has just entered scale and nothing else
				printf("%d\n", scale); // print the existing scale
				continue;
			}

			for (i = 5; str[i]; i++) { 
				if (str[i] == ' ' || str[i] == '\t') {

				} else if (str[i] == '=') { // if there is = after scale 
					flag = 1;
				} else if (str[i] >= '0' && str[i] <= '9') {
					if (flag) { // if '=' has been encountered
						flag = 0;
						scale = str[i] - '0';
						if (str[i + 1] >= '0' && str[i + 1] <= '9') { // if number is multidigit
							do {
								i++;
								scale = scale * 10 + (str[i] - '0');
							} while (str[i + 1] >= '0' && str[i + 1] <= '9');
						}
					} else { // if number is encountered first than =
						printf("Invalid Scale\n");
						break;
					}
				} else {
					printf("Invalid Scale\n");
					break;
				}
			}
	
		} else if (!strcmp(str, "quit")) { // when quit is typed end the program
			break;

		} else if (!strcmp(str, "clear")) { // to clear screen
			system("clear");

		} else if (infixEval(str, length, scale) == INT_MIN) { // evaluate the expression
			printf("Invalid Expression\n");
		}

	} while (1);

	free(str);

	return 0;
}

/*
 * readLine():
 * Pre-condition:
 * 	Takes a pointer to character pointer as input
 * Post-condition:
 * 	Takes user input in the character pointer which is passed to the function
 * RETURN VALUES:
 * 	On Success it returns the number of characters read
 * 	On Failure it returns INT_MIN 
 */

int readLine(char **ptr) {
	int i;
	*ptr = (char*) malloc(sizeof(char));
	if (*ptr == NULL)
		return INT_MIN;

	for (i = 0; ((*ptr)[i] = getchar()) != '\n'; i++)
		(*ptr) = (char*) realloc(*ptr, (i + 2) * sizeof(char));	

	(*ptr)[i] = '\0';
	return i;
}
