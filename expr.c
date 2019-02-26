#include "lexer.h"
#include "stack.h"
#include "parse.h"
#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// begain expression functions
Lexeme *expression(void)
{
    Lexeme *u = unary();
    if (binaryOperatorPending())
    {
        Lexeme *op, *op1, *op2;
        Stack *operands  = newStack();
        Stack *operators = newStack();
        pushStack(operands, u);
        while (binaryOperatorPending())
        {
            if (isEmptyStack(operators))
            {
                op = binaryOperator();
                pushStack(operators, op);

                u = unary();
                pushStack(operands, u);
            }
            else
            {
                op1 = peekStack(operators);
                op2 = binaryOperator();
                while (  isLeftAssociative(op1))
                {
                    op = popStack(operators);
                    op->right = popStack(operands);
                    op->left  = popStack(operands);
                    pushStack(operands, op);

                    if (isEmptyStack(operators)){
                        break;
                    }
                    else
                        op1 = peekStack(operators);
                }
                pushStack(operators, op2);
                u = unary();
                pushStack(operands, u);
            }
        }
        while (sizeStack(operands) != 1)
        {
            op = popStack(operators);
            op->right = popStack(operands);
            op->left  = popStack(operands);
            pushStack(operands, op);
        }
        u = popStack(operands);
        freeStack(operands);
        freeStack(operators);
    }
    return u;
}
Lexeme *returnExpression(void)
{
    Lexeme *r = advance();

    if (check(COMMA) || check(END_DEFINE)){
        r->right = newLexeme(UNINITIALIZED);
    }
    else{
        r->right = expression();
    }
    r->left = falseLexeme();
    return r;
}
Lexeme *expressionFinisher(Lexeme *v)
{
    Lexeme *u = postChecker(v);

    if (binaryOperatorPending())
    {
        Lexeme *op, *op1, *op2;

        Stack *operands  = newStack();
        Stack *operators = newStack();

        pushStack(operands, u);

        while (binaryOperatorPending())
        {
            if (isEmptyStack(operators))
            {
                op = binaryOperator();
                pushStack(operators, op);

                u = unary();
                pushStack(operands, u);
            }
            else
            {

                op1 = peekStack(operators);
                op2 = binaryOperator();
                while (isLeftAssociative(op1))
                {
                    op = popStack(operators);
                    op->right = popStack(operands);
                    op->left = popStack(operands);
                    pushStack(operands, op);

                    if (isEmptyStack(operators)){
                        break;
                    }
                    else{
                        op1 = peekStack(operators);
                    }
                }
                pushStack(operators, op2);
                u = unary();
                pushStack(operands, u);
            }
        }
        while (sizeStack(operands) != 1)
        {
            op = popStack(operators);
            op->right = popStack(operands);
            op->left  = popStack(operands);
            pushStack(operands, op);
        }
        u = popStack(operands);
        freeStack(operands);
        freeStack(operators);
    }
    return u;
}
Lexeme *variable(void)
{
    Lexeme *var = advance();
    return postChecker(var);
}
Lexeme *postChecker(Lexeme *unary_head)
{
    if (check(OPEN_PAREN)){
        return functionCall(unary_head);
    }
    else if (check(OPEN_BRACKET)){
        return arrayAccess(unary_head);
    }
    else{
        return unary_head;
    }
}

Lexeme *arrayAccess(Lexeme *unary_head)
{
    Lexeme *a = newLexeme(ARRAY_ACCESS);
    a->line_number = current_lexeme->line_number;
    advance();
    a->left  = unary_head;
    a->right = body();
    match(CLOSE_BRACKET);
    return postChecker(a);
}

Lexeme *functionCall(Lexeme *unary_head)
{
    Lexeme *f = newLexeme(FUNCTION_CALL);
    f->line_number = current_lexeme->line_number;
    advance();
    f->left  = unary_head;
    f->right = expressionList(CLOSE_PAREN);
    return postChecker(f);
}

Lexeme *parenthesizedExpression(void)
{

    Lexeme *p = newLexeme(PARENTHESIS);
    p->line_number = current_lexeme->line_number;
    advance();
    p->right = expression();
    match(CLOSE_PAREN);
    return postChecker(p);
}

Lexeme *blockExpression(void)
{
    Lexeme *b = newLexeme(CURLY_BRACES);
    b->line_number = current_lexeme->line_number;
    advance();
    b->right = body();
    match(CLOSE_BRACE);
    return postChecker(b);
}

Lexeme *ifExpression(void)
{
    Lexeme *f = advance();

    f->right = ifCases();
    return postChecker(f);
}

Lexeme *ifCases(void)
{
    match(OPEN_BRACKET);
    Lexeme *root = newLexeme(LINK);
    root->left = ifCase();
    Lexeme *current = root;
    int found_else_case = 0;
    while (check(COLON) && (found_else_case == 0))
    {
        advance();
        current->right = newLexeme(LINK);
        current = current->right;
        if (check(ELSE))
        {
            current->left = elseCase();
            found_else_case = 1;
        }
        else{
            current->left = ifCase();
        }
    }
    match(CLOSE_BRACKET);
    current->right = nilLexeme();
    return root;
}

Lexeme *ifCase(void)
{
    Lexeme *c = newLexeme(CASE);
    c->left  = body();
    match(QUESTION_MARK);
    c->right = body();
    return c;
}

Lexeme *elseCase(void)
{
    Lexeme *c = newLexeme(CASE);
    c->left   = advance();
    c->right  = body();
    return c;
}

Lexeme *whileExpression(void)
{
    Lexeme *w = advance();
    match(OPEN_BRACKET);

    w->right = newLexeme(WHILE_SPLIT);
    w->right->left = newLexeme(BODY);
    w->right->left->right = expressionList(COLON);

    w->left = expression();
    match(COLON);
    w->right->right = newLexeme(BODY);
    w->right->right->right = expressionList(CLOSE_BRACKET);

    return postChecker(w);
}

Lexeme *lambdaExpression(void)
{
    Lexeme *l = advance();
    match(OPEN_BRACKET);
    l->left  = variableList(COLON);
    l->right = body();
    match(CLOSE_BRACKET);
    return postChecker(l);
}
char *makeCopy(const char *src)
{
    char *dest = malloc((1 + strlen(src)) * sizeof(char));
    strcpy(dest, src);
    return dest;
}

void checkArg(int argcount, char *func, Lexeme *arglist)
{
    int i = 0;
    while (i < argcount)
    {
        if (arglist->type == NIL)
        {
            fprintf(stderr, "%s: not enough arguments!\n", func);
            exit(EXIT_FAILURE);
        }
        arglist = arglist->right;
        ++i;
    }

    if (arglist->type != NIL)
    {
        fprintf(stderr, "%s: too many arguments!\n", func);
        exit(EXIT_FAILURE);
    }
}
