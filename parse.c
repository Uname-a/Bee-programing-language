//mgbaird
//some parts are based on reading material provided by Dr. Lusth

// mostly tree based with a stack implimatation for the expression parser

//expression fuctions are in expr.h,.c to reduce file size
#include "type.h"
#include "lexer.h"
#include "stack.h"
#include "parse.h"
#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//setup and base fuctions
Lexeme *parseProgram(char *filename)
{
    parse_fp = lexFopen(filename);
    if (parse_fp == NULL)
    {
        perror("Failed: ");
        exit(EXIT_FAILURE);
    }
    advance();
    Lexeme *parse_tree = statements();
    return parse_tree;
}

int check(Type type)
{
    return current_lexeme->type == type;
}

Lexeme *advance(void)
{
    Lexeme *temporary = current_lexeme;
    current_lexeme    = lex(parse_fp);
    return temporary;
}

Lexeme *match(Type type)
{
    matchWithoutAdvance(type);
    return advance();
}

void matchWithoutAdvance(Type type)
{
    if (!check(type))
    {
        fprintf(stderr, "%s does not match  %s \n\n",
                lexemeInfo(current_lexeme), typeToString(type));
        exit(EXIT_FAILURE);
    }
}

Lexeme *body(void)
{
    Lexeme *body = newLexeme(BODY);
    body->right  = newLexeme(LINK);

    Lexeme *current = body->right;
    int is_expression = 0;
    current->left   = statementOrExpression(&is_expression);

    while (is_expression == 0)
    {
        current->right = newLexeme(LINK);
        current = current->right;
        current->left  = statementOrExpression(&is_expression);
    }
    current->right = nilLexeme();
    return body;
}

Lexeme *statements(void)
{
    if (check(END_OF_INPUT)) {
        return nilLexeme();
    }
    Lexeme *root = newLexeme(LINK);
    int is_expression = 0;
    root->left   = statementOrExpression(&is_expression);

    Lexeme *current = root;

    while (!check(END_OF_INPUT) && (is_expression == 0))
    {
        current->right = newLexeme(LINK);
        current = current->right;
        current->left  = statementOrExpression(&is_expression);
    }
    if (is_expression == 1)
    {
        Lexeme *mark = current;
        while (mark->left != NULL) mark = mark->left;
        fprintf(stderr, "Statement at (line %d) missing a ending comma. \n\n",
                mark->line_number);
        exit(EXIT_FAILURE);
    }
    current->right = nilLexeme();

    return root;
}

//variable definitions
Lexeme *variableDeclaration(Lexeme *var)
{
    advance();
    Lexeme *d = newLexeme(DEFINE);
    d->line_number = var->line_number;
    d->left  = var;
    d->right = newLexeme(UNINITIALIZED);
    return d;
}

Lexeme *variableDefinition(Lexeme *var)
{
    Lexeme *d = advance();
    d->left   = var;
    d->right  = body();
    match(END_DEFINE);
    return d;
}


Lexeme *functionDefinition(Lexeme *var, Lexeme *list)
{
    Lexeme *d = advance();
    d->left   = var;
    d->line_number = var->line_number;
    Lexeme *lambda = d->right = newLexeme(LAMBDA);
    lambda->left  = list;
    lambda->right = body();
    match(END_DEFINE);
    return d;
}

Lexeme *statementOrExpression(int *is_expression)
{
    *is_expression = 1;

    if (check(VARIABLE))
    {
        Lexeme *var = advance();
        if (check(SEMICOLON))
        {
            *is_expression = 0;
            return variableDeclaration(var);
        }
        if (check(DEFINE))
        {
            *is_expression = 0;
            return variableDefinition(var);
        }
        if (check(OPEN_PAREN))
        {
            advance();

            int nonvariable;
            Lexeme *list = variableOrExpressionList(&nonvariable, CLOSE_PAREN);

            if (check(DEFINE))
            {
                if (nonvariable != 0)
                {
                    fprintf(stderr, "(line %d) To declare %s a function, must contain only variables. \n\n", var->line_number,
                            lexemeInfo(var));
                    exit(EXIT_FAILURE);
                }
                *is_expression = 0;
                return functionDefinition(var, list);
            }
            else
            {
                Lexeme *f = newLexeme(FUNCTION_CALL);
                f->line_number = var->line_number;
                f->left  = var;
                f->right = list;

                Lexeme *e = expressionFinisher(f);

                if (check(COMMA))
                {
                    advance();
                    *is_expression = 0;
                }
                return e;
            }
        }
        else
        {
            Lexeme *e = expressionFinisher(var);
            if (check(COMMA))
            {
                advance();
                *is_expression = 0;
            }
            return e;
        }
    }
    else
    {
        Lexeme *e = expression();
        if (check(COMMA))
        {
            advance();
            *is_expression = 0;
        }
        return e;
    }
}

//variable_or_expression_list and its helpers
Lexeme *variableOrExpressionList(int *nonvariable, Type delimiter)
{
    *nonvariable = 0;
    if (check(delimiter))
    {
        advance();
        return nilLexeme();
    }

    Lexeme *root = newLexeme(LINK);
    root->left   = expression();

    if (root->left->type != VARIABLE) *nonvariable = 1;
    Lexeme *current = root;
    while (check(COMMA))
    {
        advance();

        current->right = newLexeme(LINK);

        current = current->right;

        current->left = expression();

        if (current->left->type != VARIABLE) {
            *nonvariable = 1;
        }
    }
    match(delimiter);
    current->right = nilLexeme();
    return root;
}
Lexeme *expressionList(Type delimiter)
{
    int x;
    return variableOrExpressionList(&x, delimiter);
}
Lexeme *variableList(Type delimiter)
{
    int nonvariable;
    Lexeme *list = variableOrExpressionList(&nonvariable, delimiter);
    if (nonvariable == 1)
    {

        fprintf(stderr, "List of variables beginning at (line %d): parameter list must only contain variables. \n\n",
                list->left->line_number);
        exit(EXIT_FAILURE);
    }
    return list;
}

//math and comparitors
Lexeme *binaryOperator(void)
{
    if (check(NOT))
        return binaryOperatorHelper();
    return advance();
}

Lexeme *binaryOperatorHelper(void)
{
    advance();
    Lexeme *l;
    if (check(EQUALS)) {
        l = newLexeme(NOT_EQUALS);
    }
    else if (check(GREATER_THAN)) {
        l = newLexeme(LESS_THAN_OR_EQUALS);
    }
    else if (check(GREATER_THAN_OR_EQUALS)) {
        l = newLexeme(LESS_THAN);
    }
    else if (check(LESS_THAN)) {
        l = newLexeme(GREATER_THAN_OR_EQUALS);
    }
    else if (check(LESS_THAN_OR_EQUALS)) {
        l = newLexeme(GREATER_THAN);
    }
    else
    {
        fprintf(stderr, "(line %d) NOT does not apply to %s :\n\n", current_lexeme->line_number,

                lexemeInfo(current_lexeme));
        exit(EXIT_FAILURE);
    }
    l->line_number = current_lexeme->line_number;
    advance();
    return l;
}
int binaryOperatorPending(void)
{
    switch (current_lexeme->type)
    {
    case ASSIGN :
    case PLUS :
    case TIMES :
    case MINUS :
    case DIVIDED_BY :
    case REMAINDER :
    case EXPONENT :
    case MOD :
    case AND :
    case OR :
    case EQUALS :
    case NOT_EQUALS :
    case LESS_THAN :
    case GREATER_THAN :
    case LESS_THAN_OR_EQUALS :
    case GREATER_THAN_OR_EQUALS :
    case NOT :
        return 1;

    default : return 0;
    }
}

//is it left associative? stay tuned and you will find out
int isLeftAssociative(Lexeme *op)//
{
    switch (op->type)
    {
    case EXPONENT :
    case ASSIGN :
        return 0;
    case AND :
    case DIVIDED_BY :
    case EQUALS :
    case GREATER_THAN :
    case GREATER_THAN_OR_EQUALS :
    case LESS_THAN :
    case LESS_THAN_OR_EQUALS :
    case MINUS :
    case MOD :
    case NOT_EQUALS :
    case OR :
    case PLUS :
    case REMAINDER :
    case TIMES :
        return 1;
    default :
        fprintf(stderr, "(line %d) \n\n", op->line_number);
        exit(EXIT_FAILURE);
    }
}

int unaryOperatorPending(void) {
    return (check(PLUS) || check(MINUS) || check(NOT));
}

Lexeme *unaryOperator(void)
{
    if (check(PLUS))
    {
        int line_number = current_lexeme->line_number;
        advance();
        Lexeme *l = newLexeme(UNARY_PLUS);
        l->line_number = line_number;
        l->right = unary();
        return l;
    }
    if (check(MINUS))
    {
        int line_number = current_lexeme->line_number;
        advance();
        Lexeme *l = newLexeme(UNARY_MINUS);
        l->line_number = line_number;
        l->right = unary();
        return l;
    }

    else
    {
        Lexeme *l = advance();
        l->right  = unary();
        return l;
    }
}

Lexeme *unary(void)
{
    if (literalPending())
    {
        Lexeme *l = advance();
        if (check(OPEN_PAREN) || check(OPEN_BRACKET))
        {
            fprintf(stderr, "type %s is not postfix operable \n\n",
                    typeToString(l->type));
            exit(EXIT_FAILURE);
        }
        return l;
    }
    if (unaryOperatorPending()) {
        return unaryOperator();
    }
    if (check(VARIABLE)) {
        return variable();
    }
    if (check(OPEN_PAREN)) {
        return parenthesizedExpression();
    }
    if (check(OPEN_BRACE)) {
        return blockExpression();
    }
    if (check(IF)) {
        return ifExpression();
    }
    if (check(WHILE)) {
        return whileExpression();
    }
    if (check(LAMBDA)) {
        return lambdaExpression();
    }
    if (check(RETURN)) {
        return returnExpression();
    }
    fprintf(stderr, "%s is not a unary. \n\n",

            lexemeInfo(current_lexeme));
    exit(EXIT_FAILURE);
}
int literalPending(void)
{
    return (check(INTEGER) || check(REAL) || check(STRING) ||
            check(TRUE) || check(FALSE) || check(THIS) || check(NIL));
}
void checkArgcount(int argcount, char *func, Lexeme *arglist)
{
    int i = 0;
    while (i < argcount)
    {
        if (arglist->type == NIL)
        {
            fprintf(stderr, "%s: not enough args\n", func);
            exit(EXIT_FAILURE);
        }
        arglist = arglist->right;
        ++i;
    }

    if (arglist->type != NIL)
    {
        fprintf(stderr, "%s: too many args\n", func);
        exit(EXIT_FAILURE);
    }
}