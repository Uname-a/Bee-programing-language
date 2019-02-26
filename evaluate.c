#include "evaluate.h"
#include "eval_cmp_math.h"
#include "eval_builtin.h"
#include "type.h"
#include "lexer.h"
#include "lexeme.h"
#include "parse.h"
#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void nilcheck(void *p, char *caller)
{
    if (p == NULL)
    {
        fprintf(stderr, "%s: NULL POINTER\n", caller);
        exit(EXIT_FAILURE);
    }
}

Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals)
{
    return cons(ENV, env, makeTable(vars, vals));
}

Lexeme *makeTable(Lexeme *vars, Lexeme *vals)
{
    return cons(TABLE, vars, vals);
}

Lexeme *createEnv(void)
{
    return extendEnv(nilLexeme(), nilLexeme(), nilLexeme());
}

Lexeme *insertEnv(Lexeme *env, Lexeme *var, Lexeme *val)
{
    Lexeme *table = cdr(env);
    setCdr(table, cons(LINK, val, cdr(table)));
    setCar(table, cons(LINK, var, car(table)));
    
    return val;
}

Lexeme *lookupEnv(Lexeme *env, Lexeme *target)
{
    Lexeme *table, *vars, *vals, *var, *val;
    while (env->type != NIL)
    {
        table = cdr(env);
        vals  = cdr(table);
        vars  = car(table);
        

        while (vars->type != NIL)
        {
            val = car(vals);
            var = car(vars);
            
            if (strcmp(target->value.string, var->value.string) == 0){
                return val;
            }
            vals = cdr(vals);
            vars = cdr(vars);
            
        }

        env = car(env);
    }
    fprintf(stderr, "lookupEnv(): Variable %s is undefined. \n", target->value.string);
    exit(EXIT_FAILURE);
}

Lexeme *lookupLocalEnv(Lexeme *env, Lexeme *target)
{
    Lexeme *table, *vars, *vals, *var, *val;

    table = cdr(env);
    vars  = car(table);
    vals  = cdr(table);

    while (vars->type != NIL)
    {
        var = car(vars);
        val = car(vals);
        if (strcmp(target->value.string, var->value.string) == 0){
            return val;
        }

        vars = cdr(vars);
        vals = cdr(vals);
    }

    fprintf(stderr, "lookupLocalEnv(): Variable %s is undefined.\n", target->value.string);
    exit(EXIT_FAILURE);
}

int checkEnv(Lexeme *env, Lexeme *target)
{
    Lexeme *table, *vars, *var;
    while (env->type != NIL)
    {
        table = cdr(env);
        vars  = car(table);
        while (vars->type != NIL)
        {
            var = car(vars);
            if (strcmp(target->value.string, var->value.string) == 0){
                return 1;
            }
            vars = cdr(vars);
        }
        env = car(env);
    }
    return 0;
}

int checkLocalEnv(Lexeme *env, Lexeme *target)
{
    Lexeme *table, *vars, *var;
    if (env->type != NIL)
    {
        table = cdr(env);
        vars  = car(table);
        while (vars->type != NIL)
        {
            var = car(vars);
            if (strcmp(target->value.string, var->value.string) == 0){
                return 1;
            }
            vars = cdr(vars);
        }
    }

    return 0;
}

Lexeme *updateEnv(Lexeme *env, Lexeme *target, Lexeme *new_val)
{
    Lexeme *table, *vars, *vals, *var;

    while (env->type != NIL)
    {
        table = cdr(env);
        vars  = car(table);
        vals  = cdr(table);
        while (vars->type != NIL)
        {
            var = car(vars);
            if (strcmp(target->value.string, var->value.string) == 0)
            {
                setCar(vals, new_val);
                return new_val;
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = car(env);
    }
    fprintf(stderr, "updateEnv(): Variable %s is undefined.\n", target->value.string);
    exit(EXIT_FAILURE);
}

Lexeme *cons(Type type, Lexeme *car, Lexeme *cdr)
{
    Lexeme *l = newLexeme(type);
    l->left   = car;
    l->right  = cdr;
    return l;
}

Lexeme *car(Lexeme *l)
{
    nilcheck(l, "car");
    return l->left;
}

Lexeme *cdr(Lexeme *l)
{
    nilcheck(l, "cdr");
    return l->right;
}

void setCar(Lexeme *l, Lexeme *car)
{
    nilcheck(l, "setCar");
    l->left = car;
}

void setCdr(Lexeme *l, Lexeme *cdr)
{
    nilcheck(l, "setCdr");
    l->right = cdr;
}

Type getType(Lexeme *l)
{
    nilcheck(l, "getType");
    return l->type;
}

Lexeme *evalVariable(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalVariable()");

    return lookupEnv(env, tree);
}

Lexeme *evalArrayAccess(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalArrayAccess()");

    Lexeme *array = eval(tree->left, env);
    if (array->type != ARRAY)
    {
        fprintf(stderr, "evalArrayAccess(): attempted to access non-array object\n");
        exit(EXIT_FAILURE);}
    Lexeme *index = eval(tree->right, env);
    if (index->type != INTEGER)
    {
        fprintf(stderr, "evalArrayAccess(): index is not an int\n");
        exit(EXIT_FAILURE);
    }
    else if (index->value.integer < 0 || index->value.integer >= array->array_size)
    {
        fprintf(stderr, "evalArrayAccess(): index (%d) is out of bounds [0,%d).\n",
                index->value.integer, array->array_size);
        exit(EXIT_FAILURE);
    }
    return array->value.array[index->value.integer];
}
Lexeme *evalDefine(Lexeme *tree, Lexeme *env)
{
    Lexeme *var = tree->left; 
    if (var->type != VARIABLE)
    {
        fprintf(stderr, "evalDefine(): attempted to define non-variable.\n");
        exit(EXIT_FAILURE);
    }
    if (checkLocalEnv(env, var))
    {
        fprintf(stderr, "line %d: evalDefine(): variable %s already defined.\n",
                var->line_number, var->value.string);
        exit(EXIT_FAILURE);
    }
    Lexeme *val = eval(tree->right, env);
    insertEnv(env, var, val);
    return nilLexeme();
}

Lexeme *evalAssign(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalAssign()");

    if (tree->left->type == ARRAY_ACCESS){
        return evalAssignArrayElement(tree, env);
    }

    Lexeme *var = tree->left; 
    if (var->type != VARIABLE)
    {
        fprintf(stderr, "evalDefine(): attempted to assign to non-variable.\n");
        exit(EXIT_FAILURE);
    }
    if (!checkEnv(env, var))
    {
        fprintf(stderr, "line %d: evalDefine(): variable %s not defined.\n",
                var->line_number, var->value.string);
        exit(EXIT_FAILURE);
    }
    Lexeme *val = eval(tree->right, env);
    return updateEnv(env, var, val);
}

Lexeme *evalAssignArrayElement(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalAssignArrayElement()");

    Lexeme *val = eval(tree->right, env);

    tree = tree->left;
    Lexeme *array = eval(tree->left, env);
    if (array->type != ARRAY)
    {
        fprintf(stderr, "evalAssignArrayElement(): attempted to access non-array object.\n");
        exit(EXIT_FAILURE);
    }
    Lexeme *index = eval(tree->right, env);
    if (index->type != INTEGER)
    {
        fprintf(stderr, "evalAssignArrayElement(): index is not an integer\n");
        exit(EXIT_FAILURE);
    }
    else if (index->value.integer < 0 || index->value.integer >= array->array_size)
    {
        fprintf(stderr, "evalAssignArrayElement(): index (%d) is out of bounds [0,%d).\n",
                index->value.integer, array->array_size);
        exit(EXIT_FAILURE);
    }
    return array->value.array[index->value.integer] = val;
}

void matchParamArgs(Lexeme *list1, Lexeme *list2)
{
    nilcheck(list2, "matchParamArgs(): arg \"list2\"");
    nilcheck(list1, "matchParamArgs(): arg \"list1\"");
    

    while (list1->type != NIL)
    {
        if (list2->type == NIL)
        {
            fprintf(stderr, "not enough args\n");
            exit(EXIT_FAILURE);
        }
        list2 = list2->right;
        list1 = list1->right;
        
    }
    if (list2->type != NIL)
    {
        fprintf(stderr, "too many args\n");
        exit(EXIT_FAILURE);
    }
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalFuncCall()");

    Lexeme *closure = eval(tree->left, env);

    if (closure->type == BUILTIN){
        return closure->value.fp(eval(tree->right, env));
    }

    if (closure->type != CLOSURE)
    {
        fprintf(stderr, "object doesn't evaluate to a closure\n");
        exit(EXIT_FAILURE);
    }

    // get components
    Lexeme *def_env  = closure->left;
    Lexeme *lambda  = closure->right;
    Lexeme *parameters = lambda->left;
    Lexeme *body  = lambda->right;
    Lexeme *arguments  = eval(tree->right, env);

    // check that there are an equal number of elements
    matchParamArgs(parameters, arguments);

    // extend environment from defining environment and populate table
    Lexeme *x_env = extendEnv(def_env, parameters, arguments);
    // evaluate body in this new environment
    Lexeme *r = eval(body, x_env);
    if (r->type == RETURN)
    {
        return r->right;
    }
    return r;
}

Lexeme *evalReturn(Lexeme *tree, Lexeme *env)
{
    if (isFalse(tree->left)) 
    {
        Lexeme *r = newLexeme(RETURN);
        r->right = eval(tree->right, env);
        r->left = trueLexeme();
        return r;
    }
    return tree;
}

void returnError(char *calling_location)
{
    fprintf(stderr, "Cannot return: %s\n", calling_location);
    exit(EXIT_FAILURE);
}

Lexeme *evalLambda(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalLambda()");

    Lexeme *closure = newLexeme(CLOSURE);
    closure->left  = env;  
    closure->right = tree; 
    return closure;
}

void initBuiltins(Lexeme *env)
{
    insertBuiltin("print", &evalPrint, env);
    insertBuiltin("println", &evalPrintln, env);
    insertBuiltin("allocate", &evalAllocate, env);
    insertBuiltin("real", &evalReal, env);
    insertBuiltin("integer", &evalInteger, env);
    insertBuiltin("list", &evalMakeList, env);
    insertBuiltin("cons", &evalCons, env);
    insertBuiltin("car", &evalCar, env);
    insertBuiltin("cdr", &evalCdr, env);
    insertBuiltin("sizeof", &evalSizeof, env);
    insertBuiltin("isNil", &evalIsNil, env);
    insertBuiltin("isString", &evalIsString, env);
    insertBuiltin("isInteger", &evalIsInteger, env);
    insertBuiltin("isReal", &evalIsReal, env);
    insertBuiltin("getArgCount", &evalGetArgCount, env);
    insertBuiltin("getArg", &evalGetArg, env);
    insertBuiltin("openFile", &evalOpenFileForReading, env);
    insertBuiltin("readInteger", &evalReadInteger, env);
    insertBuiltin("atFileEnd", &evalAtFileEnd, env);
    insertBuiltin("closeFile", &evalCloseFile, env);
}

Lexeme *insertBuiltin(char *var_name, Lexeme * (*fp)(Lexeme *), Lexeme *env)
{
    Lexeme *var, *val;

    var = newLexeme(VARIABLE);
    var->value.string = var_name;
    val = newLexeme(BUILTIN);
    val->value.fp = fp;

    return insertEnv(env, var, val);
}

Lexeme *evalList(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalList()");

    if (tree->type == NIL){
        return nilLexeme();
    }

    Lexeme *result_root = newLexeme(LINK);


    Lexeme *result = result_root;
    Lexeme *current = tree;

    result->left = eval(current->left, env);


    current = current->right;

    while (current->type != NIL)
    {
        result->right = newLexeme(LINK);
        result = result->right;

        result->left = eval(current->left, env);

        current = current->right;
    }
    result->right = nilLexeme();

    return result_root;
}

Lexeme *evalBody(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "eval_body()");

    Lexeme *result = NULL;
    Lexeme *current = tree->right;
    while (current->type != NIL && (result == NULL || result->type != RETURN))
    {
        result = eval(current->left, env);
        current = current->right;
    }
    return result;
}

Lexeme *evalParen(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "eval_paren()");
    return eval(tree->right, env);
}

Lexeme *evalBlock(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "eval_block()");
    Lexeme *block_env = extendEnv(env, nilLexeme(), nilLexeme());
    return eval(tree->right, block_env);
}

Lexeme *evalIf(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalIf()");

    Lexeme *caseList = tree->right;

    Lexeme *currentCase;
    Lexeme *condition, *truth, *body;

    while (caseList->type != NIL)
    {
        currentCase = caseList->left;
        condition = currentCase->left;
        body      = currentCase->right;

        truth = eval(condition, env);
        if (isTrue(truth) && truth->type != RETURN){
            return eval(body, env);
        }
        else if (truth->type == RETURN){
            returnError("if");
        }

        caseList = caseList->right;
    }

    return newLexeme(UNINITIALIZED);
}

Lexeme *evalWhile(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "evalWhile()");

    Lexeme *precond   = tree->right->left;
    Lexeme *condition = tree->left;
    Lexeme *postcond  = tree->right->right;

    Lexeme *val = newLexeme(UNINITIALIZED);

    if (precond->right->type != NIL)
        val = eval(precond, env);

    Lexeme *truth = eval(condition, env);
    while (isTrue(truth) && truth->type != RETURN)
    {
        if (precond->right->type  != NIL){
            val = eval(precond, env);
        }
        if (postcond->right->type != NIL){
            val = eval(postcond, env);
        }
        if (truth->type == RETURN){
            returnError("while");
        }
        
        
        truth = eval(condition, env);
    }

    return val;
}
int isTrue(Lexeme *l)
{
    return l->type == TRUE;
}

int isFalse(Lexeme *l)
{
    return l->type == FALSE;
}

Lexeme *evalNot(Lexeme *tree, Lexeme *env)
{
    Lexeme *operand = eval(tree->right, env);

    if (isTrue(operand)){
        return falseLexeme();
    }
    else{
        return trueLexeme();
    }
}

Lexeme *evalAnd(Lexeme *tree, Lexeme *env)
{
    Lexeme *lOperand = eval(tree->left, env);

    if (isFalse(lOperand)){
        return falseLexeme();
    }

    Lexeme *rOperand = eval(tree->left, env);

    if (isFalse(rOperand)){
        return falseLexeme();
    }

    return trueLexeme();
}

Lexeme *evalOr(Lexeme *tree, Lexeme *env)
{
    Lexeme *lOperand = eval(tree->left, env);

    if (isTrue(lOperand)){
        return trueLexeme();
    }

    Lexeme *rOperand = eval(tree->left, env);

    if (isTrue(rOperand)){
        return trueLexeme();
    }

    return falseLexeme();
}
Lexeme *eval(Lexeme *tree, Lexeme *env)
{
    nilcheck(tree, "eval()");

    switch (tree->type)
    {
    // literals/self-evaluated
    case INTEGER:       return tree;
    case REAL:          return tree;
    case TRUE:          return tree;
    case FALSE:         return tree;
    case STRING:        return tree;
    case CLOSURE:       return tree;
    case UNINITIALIZED: return tree;
    case NIL:           return tree;
    case ARRAY:         return tree;

    // pseudo-literals
    case THIS:          return env;
    case ELSE:          return trueLexeme();

    // math
    case PLUS:       return evalPlus(tree, env);
    case MINUS:      return evalMinus(tree, env);
    case TIMES:      return evalTimes(tree, env);
    case DIVIDED_BY: return evalDivide(tree, env);
    case EXPONENT:   return evalPow(tree, env);
    case REMAINDER:  return evalRemainder(tree, env);
    case MOD:        return evalMod(tree, env);
    case UNARY_PLUS:     return evalUnaryPlus(tree, env);
    case UNARY_MINUS:    return evalUnaryMinus(tree, env);

    // variables
    case VARIABLE:     return evalVariable(tree, env);
    case ARRAY_ACCESS: return evalArrayAccess(tree, env);

    // definitions and assignment
    case DEFINE: return evalDefine(tree, env);
    case ASSIGN: return evalAssign(tree, env);

    // function call
    case FUNCTION_CALL: return evalFuncCall(tree, env);

    // lambda
    case LAMBDA: return evalLambda(tree, env);

    // relational
    case EQUALS:                 return evalEquals(tree, env);
    case NOT_EQUALS:             return evalNeq(tree, env);
    case LESS_THAN:              return evalLt(tree, env);
    case GREATER_THAN:           return evalGt(tree, env);
    case LESS_THAN_OR_EQUALS:    return evalLtOrEq(tree, env);
    case GREATER_THAN_OR_EQUALS: return evalGtOrEq(tree, env);

    // logical
    case NOT: return evalNot(tree, env);
    case AND: return evalAnd(tree, env);
    case OR:  return evalOr(tree, env);

    // conditional
    case IF: return evalIf(tree, env);

    // loop
    case WHILE: return evalWhile(tree, env);

    // compound
    case LINK: return evalList(tree, env);
    case BODY: return evalBody(tree, env);
    case PARENTHESIS:  return evalParen(tree, env);
    case CURLY_BRACES: return evalBlock(tree, env);

    case RETURN: return evalReturn(tree, env);

    default: break;
    }

    fprintf(stderr, "eval: INVALID LEXEME.\n");
    exit(EXIT_FAILURE);
}