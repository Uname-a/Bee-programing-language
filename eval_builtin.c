#include "eval_builtin.h"
#include "evaluate.h"
#include "type.h"
#include "lexeme.h"
#include "lexer.h"
#include "stack.h"
#include "parse.h"
#include "expr.h"
#include <stdio.h>
#include <stdlib.h>

static int comma = 0;

Lexeme *evalAllocate(Lexeme *arglist)
{

    nilcheck(arglist, "evalAllocate()");
    checkArgcount(1, "allocate()", arglist);
    Lexeme *arg = arglist->left;
    if (arg->type != INTEGER)
    {
        fprintf(stderr, "allocate(): cannot allocate non-integer");
        exit(EXIT_FAILURE);
    }

    Lexeme *array = newLexeme(ARRAY);
    array->array_size  = arg->value.integer;
    array->value.array = malloc(array->array_size * sizeof(Lexeme *));
    int i = 0;
    for (i = 0; i < array->array_size; ++i)
    {
        array->value.array[i] = newLexeme(UNINITIALIZED);
    }
    return array;
}

Lexeme *evalReal(Lexeme *arglist)
{
    nilcheck(arglist, "evalReal()");

    checkArgcount(1, "real()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == REAL)
    {
        return arg;
    }
    if (arg->type == INTEGER)
    {
        arg->type = REAL;
        arg->value.real = (double) arg->value.integer;
        return arg;
    }
    fprintf(stderr, "real(): argument cannot be coerced to type real");
    exit(EXIT_FAILURE);
}

Lexeme *evalInteger(Lexeme *arglist)
{
    nilcheck(arglist, "evalInteger()");

    checkArgcount(1, "integer()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == INTEGER) {

        return arg;
    }
    if (arg->type == REAL)
    {
        arg->type = INTEGER;
        arg->value.integer = (int) arg->value.real;
        return arg;
    }

    fprintf(stderr, "integer(): argument cannot be changed to an integer");
    exit(EXIT_FAILURE);
}

Lexeme *evalMakeList(Lexeme *arglist)
{
    nilcheck(arglist, "evalMakeList()");
    return arglist;
}

Lexeme *evalCons(Lexeme *arglist)
{
    nilcheck(arglist, "eval_cons()");

    checkArgcount(2, "cons()", arglist);
    Lexeme *cdr = arglist->right->left;
    Lexeme *car = arglist->left;


    Lexeme *pair = newLexeme(LINK);
    pair->right = cdr;
    pair->left  = car;

    return pair;
}

Lexeme *evalCar(Lexeme *arglist)
{

    nilcheck(arglist, "evalCar()");
    checkArgcount(1, "car()", arglist);
    Lexeme *pair = arglist->left;
    if (pair->type != LINK)
    {
        fprintf(stderr, "car(): argument not a pair");
        exit(EXIT_FAILURE);
    }
    return pair->left;
}
Lexeme *evalPrint(Lexeme *arglist)
{
    nilcheck(arglist, "evalPrint()");

    Lexeme *arg;
    while (arglist->type != NIL)
    {
        arg = arglist->left;

        switch (arg->type)// print stored value
        {
        case INTEGER:
            printf("%d", arg->value.integer);
            break;
        case REAL:
            printf("%f", arg->value.real);
            break;
        case STRING:
            printf("%s", arg->value.string);
            break;
        case NIL:
            printf("nil");
            break;
        case CLOSURE:
            printf("closure");
            break;
        case TRUE :
            printf("True");
            break;
        case FALSE :
            printf("False");
            break;

        case LINK:
        {
            int temp = comma;
            comma = 1;
            printf("(");
            evalPrint(arg);
            printf(")");
            comma = temp;
        }
        break;
        default:
            fprintf(stderr, "print: argument not of printable type");
            exit(EXIT_FAILURE);
        }
        if (comma == 1 && arglist->right->type != NIL)
            printf(", ");

        arglist = arglist->right;
    }

    return newLexeme(UNINITIALIZED);
}

Lexeme *evalPrintln(Lexeme *arglist)
{
    nilcheck(arglist, "evalPrintln()");
    Lexeme *temp = evalPrint(arglist);
    printf("\n");
    return temp;
}
Lexeme *evalCdr(Lexeme *arglist)
{

    checkArgcount(1, "cdr()", arglist);
    nilcheck(arglist, "evalCdr()");
    Lexeme *pair = arglist->left;

    if (pair->type != LINK)
    {
        fprintf(stderr, "cdr(): argument not a pair");
        exit(EXIT_FAILURE);
    }
    return pair->right;
}

Lexeme *evalSizeof(Lexeme *arglist)
{

    checkArgcount(1, "sizeof()", arglist);
    nilcheck(arglist, "sizeof()");
    Lexeme *array = arglist->left;
    if (array->type != ARRAY)
    {
        fprintf(stderr, "sizeof(): argument not a array");
        exit(EXIT_FAILURE);
    }
    Lexeme *size = newLexeme(INTEGER);
    size->value.integer = array->array_size;

    return size;
}


Lexeme *evalIsReal(Lexeme *arglist)
{
    nilcheck(arglist, "isReal()"); checkArgcount(1, "isReal()", arglist);
    Lexeme *object = arglist->left;
    if (object->type == REAL)
        return trueLexeme();
    else
        return falseLexeme();
}

Lexeme *evalIsInteger(Lexeme *arglist)
{
    nilcheck(arglist, "isInteger()");
    checkArgcount(1, "isInteger()", arglist);
    Lexeme *object = arglist->left;
    if (object->type == INTEGER)
        return trueLexeme();
    else
        return falseLexeme();
}

Lexeme *evalIsString(Lexeme *arglist)
{
    nilcheck(arglist, "isString()");
    checkArgcount(1, "isString()", arglist);

    Lexeme *object = arglist->left;
    if (object->type == STRING)
        return trueLexeme();
    else
        return falseLexeme();
}

Lexeme *evalIsNil(Lexeme *arglist)
{
    nilcheck(arglist, "isNil()");
    checkArgcount(1, "isNil()", arglist);

    Lexeme *object = arglist->left;
    if (object->type == NIL)
        return trueLexeme();
    else
        return falseLexeme();
}

Lexeme *evalGetArgCount()
{
    return newLexemeInteger(INTEGER, countCL);
}

Lexeme *evalGetArg(Lexeme *arglist)
{
    nilcheck(arglist, "GetArg()");
    checkArgcount(1, "GetArg()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == INTEGER) {
        return newLexemeString(STRING, argsCL[arg->value.integer]);
    }
    fprintf(stderr, "GetArg(): argument not a type integer");
    exit(EXIT_FAILURE);
}

Lexeme *evalOpenFileForReading(Lexeme *arglist)
{
    nilcheck(arglist, "OpenFile()");
    checkArgcount(1, "OpenFile()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == STRING) {
        Lexeme *fp = newLexeme(FILE_POINTER);
        fp->value.fval = fopen(arg->value.string, "read");
        return fp;
    }
    fprintf(stderr, "evalOpenFileForReading(): argument not a type String");
    exit(EXIT_FAILURE);
}

Lexeme *evalReadInteger(Lexeme *arglist)
{

    nilcheck(arglist, "ReadInteger()");
    checkArgcount(1, "ReadInteger()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == FILE_POINTER) {
        int i;
        fscanf(arg->value.fval, "%d", &i);

        return newLexemeInteger(INTEGER, i);//readInt(arg->value.fval));
    }
    fprintf(stderr, "evalOpenFileForReading(): argument not a type FILE_POINTER");
    exit(EXIT_FAILURE);
}

Lexeme *evalAtFileEnd(Lexeme *arglist)
{

    nilcheck(arglist, "AtFileEnd()");

    checkArgcount(1, "AtFileEnd()", arglist);

    Lexeme *arg = arglist->left;
    if (arg->type == FILE_POINTER) {
        if (feof(arg->value.fval)) {        //implementation language to check EOF
            return newLexeme(TRUE);
        }
        else {
            return newLexeme(FALSE);
        }
    }
    fprintf(stderr, "AtFileEnd(): argument not a type FILE_POINTER");
    exit(EXIT_FAILURE);

}


Lexeme *evalCloseFile(Lexeme *arglist)
{
    nilcheck(arglist, "CloseFile()");
    checkArgcount(1, "CloseFile()", arglist);
    Lexeme *arg = arglist->left;
    if (arg->type == FILE_POINTER) {
        fclose(arg->value.fval); //implementation language open
        return newLexeme(TRUE);
    }
    fprintf(stderr, "evalCloseFile(): argument not a type FILE_POINTER");
    exit(EXIT_FAILURE);
}