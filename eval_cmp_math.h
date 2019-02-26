#ifndef EVAL_CMP_MATH_H
#define EVAL_CMP_MATH_H
#include "type.h"
#include "lexer.h"
#include "lexeme.h"
//basic math
Lexeme *evalPlus(Lexeme *tree, Lexeme *env);
Lexeme *evalMinus(Lexeme *tree, Lexeme *env);
Lexeme *evalTimes(Lexeme *tree, Lexeme *env);
Lexeme *evalDivide(Lexeme *tree, Lexeme *env);
//advanced math
Lexeme *evalPow(Lexeme *tree, Lexeme *env);
Lexeme *evalRemainder(Lexeme *tree, Lexeme *env);
Lexeme *evalDiv(Lexeme *tree, Lexeme *env);
Lexeme *evalMod(Lexeme *tree, Lexeme *env);
Lexeme *evalUnaryPlus(Lexeme *tree, Lexeme *env);
Lexeme *evalUnaryMinus(Lexeme *tree, Lexeme *env);
//compare
Lexeme *evalEquals(Lexeme *tree, Lexeme *env);
Lexeme *evalNeq(Lexeme *tree, Lexeme *env);
Lexeme *evalLt(Lexeme *tree, Lexeme *env);
Lexeme *evalGt(Lexeme *tree, Lexeme *env);
Lexeme *evalLtOrEq(Lexeme *tree, Lexeme *env);
Lexeme *evalGtOrEq(Lexeme *tree, Lexeme *env);
#endif
