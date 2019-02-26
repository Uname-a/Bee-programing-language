#include "evaluate.h"
#include "eval_cmp_math.h"
#include "type.h"
#include "lexer.h"
#include "stack.h"
#include "parse.h"
#include "expr.h"
#include "lexeme.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


Lexeme *evalPlus(Lexeme *tree, Lexeme *env)
{
    Lexeme *aug = eval(tree->left, env);
    Lexeme *added = eval(tree->right, env);
    Lexeme *result = newLexeme(UNINITIALIZED);
    if (aug->type == TRUE)
    {
        return trueLexeme();
    }
    if (added->type == TRUE) {
        return trueLexeme();
    }
    if (aug->type == FALSE && added->type == FALSE)
    {
        return falseLexeme();
    }

    if (aug->type == REAL && added->type == REAL)
    {
        result->type = REAL;
        result->value.real = aug->value.real + added->value.real;
        return result;
    }
    if (aug->type == REAL && added->type == INTEGER)
    {
        result->type = REAL;
        result->value.real = aug->value.real + (double) added->value.integer;
        return result;
    }

    if (aug->type == INTEGER && added->type == REAL)
    {
        result->type = REAL;
        result->value.real = (double) aug->value.integer + added->value.real;
        return result;
    }
    if (aug->type == STRING && added->type == STRING)
    {
        Lexeme *s = newLexeme(STRING);
        int length = strlen(aug->value.string) + strlen(added->value.string) + 1;
        s->value.string = malloc(length * sizeof(char));
        s->value.string[0] = '\0';
        s->value.string = strcat(s->value.string, aug->value.string);
        s->value.string = strcat(s->value.string, added->value.string);
        return s;
    }
    if (aug->type == INTEGER && added->type == INTEGER)
    {
        result->type = INTEGER;
        result->value.integer = aug->value.integer + added->value.integer;
        return result;
    }


    fprintf(stderr, "+: invalid operand");
    exit(EXIT_FAILURE);
}


Lexeme *evalMinus(Lexeme *tree, Lexeme *env)
{
    Lexeme *minu    = eval(tree->left, env);
    Lexeme *subtrahend = eval(tree->right, env);
    Lexeme *result = newLexeme(UNINITIALIZED);
    if (minu->type == REAL && subtrahend->type == INTEGER)
    {
        result->type = REAL;
        result->value.real = minu->value.real - (double) subtrahend->value.integer;
        return result;
    }
    if (minu->type == INTEGER && subtrahend->type == INTEGER)
    {
        result->type = INTEGER;
        result->value.integer = minu->value.integer - subtrahend->value.integer;
        return result;
    }

    if (minu->type == REAL && subtrahend->type == REAL)
    {
        result->type = REAL;
        result->value.real = minu->value.real - subtrahend->value.real;
        return result;
    }
    if (minu->type == INTEGER && subtrahend->type == REAL)
    {
        result->type = REAL;
        result->value.real = (double) minu->value.integer - subtrahend->value.real;
        return result;
    }

    fprintf(stderr, "-: invalid operand ");
    exit(EXIT_FAILURE);
}


Lexeme *evalTimes(Lexeme *tree, Lexeme *env)
{
    Lexeme *multi = eval(tree->left, env);
    Lexeme *result = newLexeme(UNINITIALIZED);
    Lexeme *multiplier   = eval(tree->right, env);
    if (multi->type == FALSE)
    {
        return falseLexeme();
    }
    if (multiplier->type == FALSE) {
        return falseLexeme();
    }
    if (multi->type == TRUE && multiplier->type == TRUE) {
        return trueLexeme();
    }
    if (multi->type == REAL && multiplier->type == INTEGER)
    {
        result->type = REAL;
        result->value.real = multi->value.real * (double) multiplier->value.integer;
        return result;
    }
    if (multi->type == INTEGER && multiplier->type == INTEGER)
    {
        result->type = INTEGER;
        result->value.integer = multi->value.integer * multiplier->value.integer;
        return result;
    }
    if (multi->type == REAL && multiplier->type == REAL)
    {
        result->type = REAL;
        result->value.real = multi->value.real * multiplier->value.real;
        return result;
    }
    if (multi->type == INTEGER && multiplier->type == REAL)
    {
        result->type = REAL;
        result->value.real = (double) multi->value.integer * multiplier->value.real;
        return result;
    }

    fprintf(stderr, "*: invalid operand");
    exit(EXIT_FAILURE);
}


Lexeme *evalDivide(Lexeme *tree, Lexeme *env)
{
    Lexeme *dived = eval(tree->left, env);
    Lexeme *div  = eval(tree->right, env);
    Lexeme *result = newLexeme(UNINITIALIZED);
    if (dived->type == REAL && div->type == INTEGER)
    {
        if (div->value.integer == 0)
        {
            fprintf(stderr, "diuvided by zero\n");
            exit(EXIT_FAILURE);
        }
        result->type = REAL;
        result->value.real = dived->value.real / (double) div->value.integer;
        return result;
    }
    if (dived->type == INTEGER && div->type == INTEGER)
    {
        if (div->value.integer == 0)
        {
            fprintf(stderr, "diuvided by zero\n");
            exit(EXIT_FAILURE);
        }
        result->type = INTEGER;
        result->value.integer = dived->value.integer / div->value.integer;
        return result;
    }

    if (dived->type == REAL && div->type == REAL)
    {
        if (div->value.real == 0)
        {
            fprintf(stderr, "diuvided by zero\n");
            exit(EXIT_FAILURE);
        }
        result->type = REAL;
        result->value.real = dived->value.real / div->value.real;
        return result;
    }

    if (dived->type == INTEGER && div->type == REAL)
    {
        if (div->value.real == 0)
        {
            fprintf(stderr, "divided by zero\n");
            exit(EXIT_FAILURE);
        }
        result->type = REAL;
        result->value.real = (double) dived->value.integer / div->value.real;
        return result;
    }
    fprintf(stderr, "/: invalid operand");
    exit(EXIT_FAILURE);
}


Lexeme *evalPow(Lexeme *tree, Lexeme *env)
{
    Lexeme *base     = eval(tree->left, env);
    Lexeme *exponent = eval(tree->right, env);
    Lexeme *result = newLexeme(REAL);

    if (base->type == REAL && exponent->type == REAL)
    {
        result->value.real = pow(base->value.real, exponent->value.real);
        return result;
    }
    if (base->type == INTEGER && exponent->type == REAL)
    {
        result->value.real = pow((double) base->value.integer, exponent->value.real);
        return result;
    }
    if (base->type == REAL && exponent->type == INTEGER)
    {
        result->value.real = pow(base->value.real, (double) exponent->value.integer);
        return result;
    }
    if (base->type == INTEGER && exponent->type == INTEGER)
    {
        result->value.real = pow((double) base->value.integer, (double) exponent->value.integer);
        return result;
    }

    fprintf(stderr, "^: invalid operand ");
    exit(EXIT_FAILURE);
    return NULL;
}


Lexeme *evalRemainder(Lexeme *tree, Lexeme *env)
{
    Lexeme *dived = eval(tree->left, env);
    Lexeme *div =  eval(tree->right, env);
    Lexeme *result = newLexeme(INTEGER);
    if (dived->type == INTEGER && div->type == INTEGER)
    {
        if (div->value.integer == 0)
        {
            fprintf(stderr, "divided by zero\n");
            exit(EXIT_FAILURE);
        }
        result->value.integer = dived->value.integer % div->value.integer;
        return result;
    }
    fprintf(stderr, "/: invalid operand ");
    exit(EXIT_FAILURE);
}


Lexeme *evalDiv(Lexeme *tree, Lexeme *env)
{
    Lexeme *dived = eval(tree->left, env);
    Lexeme *div =  eval(tree->right, env);
    Lexeme *result = newLexeme(INTEGER);
    if (dived->type == INTEGER && div->type == INTEGER)
    {
        if (div->value.integer < 0)
        {
            result->value.integer = (int) ceil(((double) dived->value.integer) / div->value.integer);
            return result;
        }
        if (div->value.integer > 0)
        {
            result->value.integer = (int) floor(((double) dived->value.integer) / div->value.integer);
            return result;
        }
        fprintf(stderr, "divided by zero\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "/: invalid operand ");
    exit(EXIT_FAILURE);
}


Lexeme *evalMod(Lexeme *tree, Lexeme *env)
{
    Lexeme *dived = eval(tree->left, env);
    Lexeme *div =  eval(tree->right, env);
    Lexeme *result = newLexeme(INTEGER);
    if (dived->type == INTEGER && div->type == INTEGER)
    {
        int quotient;
        if (div->value.integer < 0)
        {
            quotient = (int) ceil(((double) dived->value.integer) / div->value.integer);
            result->value.integer = dived->value.integer - (div->value.integer * quotient);
            return result;
        }
        if (div->value.integer > 0)
        {
            quotient = (int) floor(((double) dived->value.integer) / div->value.integer);
            result->value.integer = dived->value.integer - (div->value.integer * quotient);
            return result;
        }

        fprintf(stderr, "divided by zero\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "/: invalid operand ");
    exit(EXIT_FAILURE);
}


Lexeme *evalUnaryPlus(Lexeme *tree, Lexeme *env)
{
    Lexeme *operand = eval(tree->right, env);
    if (operand->type == INTEGER || operand->type == REAL)
    {
        return operand;
    }
    fprintf(stderr, "unary +: invalid operand ");
    exit(EXIT_FAILURE);
}


Lexeme *evalUnaryMinus(Lexeme *tree, Lexeme *env)
{
    Lexeme *operand = eval(tree->right, env);
    Lexeme *result = newLexeme(UNINITIALIZED);
    if (operand->type == REAL)
    {
        result->type = REAL;
        result->value.real = -1 * operand->value.real;
        return result;
    }
    if (operand->type == INTEGER)
    {
        result->type = INTEGER;
        result->value.integer = -1 * operand->value.integer;
        return result;
    }
    fprintf(stderr, "unary -: invalid operand ");
    exit(EXIT_FAILURE);
}

Lexeme *evalEquals(Lexeme *tree, Lexeme *env)
{
    Lexeme *lOperand = eval(tree->left, env);
    Lexeme *rOperand = eval(tree->right, env);
    if (lOperand->type == INTEGER && rOperand->type == REAL)
    {
        if ((double) lOperand->value.integer != rOperand->value.real) {
            return falseLexeme();
        }
        else {
            return trueLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == INTEGER)
    {
        if (lOperand->value.real != (double) rOperand->value.integer) {
            return falseLexeme();
        }
        else {
            return trueLexeme();
        }
    }
    if (lOperand->type == INTEGER && rOperand->type == INTEGER)
    {
        if (lOperand->value.integer != rOperand->value.integer) {
            return falseLexeme();
        }
        else {
            return trueLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == REAL)
    {
        if (lOperand->value.real != rOperand->value.real) {
            return falseLexeme();
        }
        else {
            return trueLexeme();
        }
    }


    if (lOperand->type == rOperand->type)
    {
        switch (lOperand->type)
        {
        case NIL:
            return trueLexeme();
        case FALSE:
            return falseLexeme();
        case TRUE:
            return trueLexeme();

        case STRING:
            if (strcmp(lOperand->value.string, rOperand->value.string) != 0) {
                return falseLexeme();
            }
            else {
                return trueLexeme();
            }
        case CLOSURE:
            if (lOperand != rOperand) {
                return falseLexeme();
            }
            else {
                return trueLexeme();
            }
        case ARRAY:
            if (lOperand->value.array != rOperand->value.array) {
                return falseLexeme();
            }
            else {
                return trueLexeme();
            }

        case UNINITIALIZED:
            fprintf(stderr, "uninitialized vars.\n");
            exit(EXIT_FAILURE);
        default:
            return falseLexeme();
        }
    }
    return falseLexeme();
}


Lexeme *evalNeq(Lexeme *tree, Lexeme *env)
{
    Lexeme *val = evalEquals(tree, env);
    if (isTrue(val))
        return falseLexeme();
    else
        return trueLexeme();
}


Lexeme *evalLt(Lexeme *tree, Lexeme *env)
{
    Lexeme *rOperand = eval(tree->right, env);
    Lexeme *lOperand = eval(tree->left, env);

    if (lOperand->type == STRING && rOperand->type == STRING)
    {
        if (strcmp(lOperand->value.string, rOperand->value.string) < 0) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == INTEGER && rOperand->type == REAL)
    {
        if ((double) lOperand->value.integer < rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == INTEGER && rOperand->type == INTEGER)
    {
        if (lOperand->value.integer < rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == REAL)
    {
        if (lOperand->value.real < rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == INTEGER)
    {
        if (lOperand->value.real < (double) rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    fprintf(stderr, "evalLt(): attempt to compare non comparable things");
    exit(EXIT_FAILURE);

}


Lexeme *evalGt(Lexeme *tree, Lexeme *env)
{
    Lexeme *rOperand = eval(tree->right, env);
    Lexeme *lOperand = eval(tree->left, env);
    if (lOperand->type == INTEGER && rOperand->type == REAL)
    {
        if ((double) lOperand->value.integer > rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == STRING && rOperand->type == STRING)
    {
        if (strcmp(lOperand->value.string, rOperand->value.string) > 0) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == INTEGER && rOperand->type == INTEGER)
    {
        if (lOperand->value.integer > rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }

    if (lOperand->type == REAL && rOperand->type == INTEGER)
    {
        if (lOperand->value.real > (double) rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == REAL)
    {
        if (lOperand->value.real > rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }




    fprintf(stderr, "evalGt():attempt to compare non comparable things");
    exit(EXIT_FAILURE);

    return NULL;
}


Lexeme *evalLtOrEq(Lexeme *tree, Lexeme *env)
{
    Lexeme *rOperand = eval(tree->right, env);
    Lexeme *lOperand = eval(tree->left, env);


    if (lOperand->type == INTEGER && rOperand->type == INTEGER)
    {
        if (lOperand->value.integer <= rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == INTEGER)
    {
        if (lOperand->value.real <= (double) rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == INTEGER && rOperand->type == REAL)
    {
        if ((double) lOperand->value.integer <= rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }

    if (lOperand->type == REAL && rOperand->type == REAL)
    {
        if (lOperand->value.real <= rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == STRING && rOperand->type == STRING)
    {
        if (strcmp(lOperand->value.string, rOperand->value.string) <= 0) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }

    fprintf(stderr, "evalLtOrEq(): attempt to compare non comparable things");
    exit(EXIT_FAILURE);

}


Lexeme *evalGtOrEq(Lexeme *tree, Lexeme *env)
{
    Lexeme *rOperand = eval(tree->right, env);
    Lexeme *lOperand = eval(tree->left, env);


    if (lOperand->type == INTEGER && rOperand->type == INTEGER)
    {
        if (lOperand->value.integer >= rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == INTEGER)
    {
        if (lOperand->value.real >= (double) rOperand->value.integer) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }

    if (lOperand->type == INTEGER && rOperand->type == REAL)
    {
        if ((double) lOperand->value.integer >= rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == STRING && rOperand->type == STRING)
    {
        if (strcmp(lOperand->value.string, rOperand->value.string) >= 0) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    if (lOperand->type == REAL && rOperand->type == REAL)
    {
        if (lOperand->value.real >= rOperand->value.real) {
            return trueLexeme();
        }
        else {
            return falseLexeme();
        }
    }
    fprintf(stderr, "evalGtOrEq(): attempt to compare non comparable things");
    exit(EXIT_FAILURE);

    return NULL;
}