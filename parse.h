// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#ifndef PARSE_H
#define PARSE_H
#include "lexeme.h"
#include "expr.h"
FILE *parse_fp;
int binaryOperatorPending(void);
int check(Type type);
int isLeftAssociative(Lexeme *op);
int literalPending(void);
int unaryOperatorPending(void);
Lexeme *advance(void);
Lexeme *binaryOperator(void);
Lexeme *binaryOperatorHelper(void);
Lexeme *body(void);
Lexeme *current_lexeme;
Lexeme *expressionList(Type delimiter);
Lexeme *functionDefinition(Lexeme *var, Lexeme *list);
Lexeme *match(Type type);
Lexeme *parseProgram(char *filename);
Lexeme *postChecker(Lexeme *unary_head);
Lexeme *statementOrExpression(int *is_expression);
Lexeme *statements(void);
Lexeme *unary(void);
Lexeme *unaryOperator(void);
Lexeme *variableDeclaration(Lexeme *var);
Lexeme *variableDefinition(Lexeme *var);
Lexeme *variableList(Type delimiter);
Lexeme *variableOrExpressionList(int *nonvariable, Type delimiter);
Lexeme *whileExpression(void);
void matchWithoutAdvance(Type type);
void checkArgcount(int argcount, char *func, Lexeme *arglist);

#endif
