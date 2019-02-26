// mgbaird
// basic stack for expression processing
#ifndef STACK_H
#define STACK_H

typedef struct node
{
	void *handle;
	struct node *next;
} Node;

typedef struct stack
{
	int size;
	Node *head;
} Stack;

Stack *newStack(void);
void pushStack(Stack *s, void *p);
void *popStack(Stack *s);
void *peekStack(Stack *s);
int isEmptyStack(Stack *s);
void freeStack(Stack *s);
int sizeStack(Stack *s);

#endif
