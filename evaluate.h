#ifndef EVALUATE_H
#define EVALUATE_H
#include "type.h"
#include "lexer.h"
#include "stack.h"
#include "parse.h"
#include "expr.h"

void nilcheck(void *p, char *caller);
Lexeme *createEnv(void);
Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals);
Lexeme *eval(Lexeme *tree, Lexeme *env);
void initBuiltins(Lexeme *env);
void returnError(char *calling_location);
// list
Lexeme *cons(Type type, Lexeme *car, Lexeme *cdr);
Lexeme *car(Lexeme *l);
Lexeme *cdr(Lexeme *l);
void setCar(Lexeme *l, Lexeme *car);
void setCdr(Lexeme *l, Lexeme *cdr);

Type getType(Lexeme *l);

// environment
Lexeme *makeTable(Lexeme *vars, Lexeme *vals);
Lexeme *createEnv(void);
Lexeme *insertEnv(Lexeme *env, Lexeme *var, Lexeme *val);
Lexeme *lookupEnv(Lexeme *env, Lexeme *var);
Lexeme *lookupLocalEnv(Lexeme *env, Lexeme *target);
int checkEnv(Lexeme *env, Lexeme *target);
int checkLocalEnv(Lexeme *env, Lexeme *target);
Lexeme *update_env(Lexeme *env, Lexeme *target, Lexeme *new_val);

// evaluate
Lexeme *evalVariable(Lexeme *tree, Lexeme *env);
Lexeme *evalArrayAccess(Lexeme *tree, Lexeme *env);
Lexeme *evalDefine(Lexeme *tree, Lexeme *env);
Lexeme *evalAssign(Lexeme *tree, Lexeme *env);
Lexeme *evalAssignArrayElement(Lexeme *tree, Lexeme *env);
Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env);
Lexeme *evalReturn(Lexeme *tree,Lexeme *env);
Lexeme *evalLambda(Lexeme *tree, Lexeme *env);

// builtin initialization
Lexeme *insertBuiltin(char *var_name, Lexeme *(*fp)(Lexeme *), Lexeme *env);

// error handling
void matchParamArgs(Lexeme *list1, Lexeme *list2);
//evaluaate 
Lexeme *evalList(Lexeme *tree,Lexeme *env); 
Lexeme *evalParen(Lexeme *tree,Lexeme *env);
Lexeme *evalBlock(Lexeme *tree,Lexeme *env);
Lexeme *evalBody(Lexeme *tree,Lexeme *env);
//if-while
Lexeme *evalIf(Lexeme *tree,Lexeme *env);
Lexeme *evalWhile(Lexeme *tree,Lexeme *env);
//logic
int isFalse(Lexeme *l);
int isTrue(Lexeme *l);
Lexeme *evalNot(Lexeme *tree,Lexeme *env);
Lexeme *evalAnd(Lexeme *tree,Lexeme *env);
Lexeme *evalOr(Lexeme *tree,Lexeme *env);
#endif
