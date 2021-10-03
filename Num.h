typedef struct node { // node of the linked list
	struct node *prev;
	char digit;
	struct node *next;
}node;

typedef struct Num {
	node *head; // pointer to first node of linked list
	node *tail; // pointer to last node of linked list
	int sign; // 0 means +ve and 1 means -ve
	int intCount; // length of the number
	int decimalCount;
}Num;

// linked list functions
void init(Num*);
int append(Num*, char);
void deleteNum(Num*);
void print(Num*);
void length(Num*);
void insertAtBeginning(Num*, char);
void insertAt(Num*, int, char);
char pop(Num*);
char popFirst(Num*);
void deleteLeadingTrailingZeroes(Num*);
int compare(Num*, Num*);
void deleteNode(Num*, char);
double NumToDouble(Num);

// operations on Num
Num add(Num*, Num*);
Num sub(Num*, Num*);
Num mul(Num*, Num*);
Num divi(Num*, Num*, int);
Num mod(Num*, Num*, int);
Num sqroot(Num*, int);
Num powe(Num, Num, int);
double s(Num);
double c(Num);
double t(Num);
int lessThan(Num*, Num*);
int greaterThan(Num*, Num*);
int equalTo(Num*, Num*);
int notEqualTo(Num*, Num*);
int lessThanEqualTo(Num*, Num*);
int greaterThanEqualTo(Num*, Num*);
int logicalNot(Num*);
int logicalAnd(Num*, Num*);
int logicalOr(Num*, Num*);
