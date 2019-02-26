//mgbaird
//the mostbasic stack for expression processing
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *newStack(void)
{
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->size = 0;
    s->head = NULL;
    return s;
}

void pushStack(Stack *s, void *p)
{
    Node *n = malloc(sizeof(Node));
    n->handle = p;
    n->next = s->head;
    s->head = n;
    (s->size)++;
}

void *popStack(Stack *s)
{
    if (isEmptyStack(s))
    {
        fprintf(stderr, "empty stack.\n");
        exit(EXIT_SUCCESS);
    }
    Node *n = s->head;
    s->head = s->head->next;
    void *p = n->handle;
    free(n);
    (s->size)--;
    return p;
}

void *peekStack(Stack *s)
{
    if (isEmptyStack(s))
    {
        fprintf(stderr, "empty stack.\n");
        exit(EXIT_SUCCESS);
    }
    return s->head->handle;
}

int isEmptyStack(Stack *s)
{
    return s->size == 0;
}

void freeStack(Stack *s)
{
    if (!isEmptyStack(s))
    {
        fprintf(stderr, "cannot free non-empty stack.\n");
        exit(EXIT_SUCCESS);
    }
    free(s);
}

int sizeStack(Stack *s)
{
    return s->size;
}
