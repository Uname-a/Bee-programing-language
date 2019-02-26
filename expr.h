// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#ifndef EXPR_H
#define EXPR_H
#include "lexeme.h"
#include "parse.h"

Lexeme *expression(void);
Lexeme *returnExpression(void);
Lexeme *expressionFinisher(Lexeme *v);
Lexeme *variable(void);
Lexeme *postChecker(Lexeme *unary_head);
Lexeme *arrayAccess(Lexeme *unary_head);
Lexeme *functionCall(Lexeme *unary_head);
Lexeme *parenthesizedExpression(void);
Lexeme *blockExpression(void);
Lexeme *ifExpression(void);
Lexeme *ifCases(void);
Lexeme *ifCase(void);
Lexeme *elseCase(void);
Lexeme *whileExpression(void);
Lexeme *lambdaExpression(void);
char *makeCopy(const char *src);
void checkArg(int argcount, char *func, Lexeme *arglist);
#endif
