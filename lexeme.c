// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#include "type.h"
#include "lexeme.h"
#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//lexeme types funcions
void initializeJustTypeLexemes(void)
{
    just_type_Lexemes = malloc(26 * sizeof(Lexeme *));
    Lexeme **jtl = just_type_Lexemes;
    jtl[TRUE] = newJustTypeLexeme(TRUE);
    jtl[FALSE] = newJustTypeLexeme(FALSE);
    jtl[NIL] = newJustTypeLexeme(NIL);
}
Lexeme *newJustTypeLexeme(Type type)
{
    Lexeme *l = malloc(sizeof(*l));
    l->line_number = getNumber();
    l->type = type;
    return l;
}

// helper fuctions
Lexeme *trueLexeme(void)
{
    return just_type_Lexemes[TRUE];
}

Lexeme *falseLexeme(void)
{
    return just_type_Lexemes[FALSE];
}

Lexeme *nilLexeme(void)
{
    return just_type_Lexemes[NIL];
}

// create a new Lexeme object

Lexeme *newLexeme(Type type)
{

    Lexeme *l = malloc(sizeof(*l));
    l->line_number = getNumber();
    l->type = type;
    l->value.integer = 0;
    l->left = NULL;
    l->right = NULL;
    return l;
}


// set lexeme types
Lexeme *newLexemeString(Type type, char *string)
{
    Lexeme *l = newLexeme(type);
    l->value.string = string;
    return l;
}

Lexeme *newLexemeInteger(Type type, int integer)
{
    Lexeme *l = newLexeme(type);
    l->value.integer = integer;
    return l;
}

Lexeme *newLexemeReal(Type type, double real)
{
    Lexeme *l = newLexeme(type);
    l->value.real = real;
    return l;
}





//main lexeme fuction for parser
char *lexemeInfo(Lexeme *l)
{
    char *s = lexemeInfoHelper(l);
    int max = 64;
    char *t =  malloc(max * sizeof(char));
    sprintf(t, " (line %d)", l->line_number);
    strcat(s, t);
    free(t);
    return s;
}

char *lexemeInfoHelper(Lexeme *l)
{
    int max = 512;
    int max_copy = 256;
    char *s = malloc(max * sizeof(char));
    switch (l->type)
    {
    case DEFINE :
        strcpy(s, "DEFINE ':='");
        return s;
    case END_DEFINE :
        strcpy(s, "END_DEFINE ':;'");
        return s;
    case ASSIGN :
        strcpy(s, "ASSIGN '=='");
        return s;
    case VARIABLE :
    {
        strcpy(s, "VARIABLE ");
        strncat(s, l->value.string, max_copy);
        return s;
    }
    case INTEGER :
    {
        char *t = malloc(max_copy * sizeof(char));
        strcpy(s, "INTEGER ");
        sprintf(t, "%d", l->value.integer);
        strncat(s, t, max_copy);
        free(t);
        return s;
    }
    case REAL :
    {
        char *t = malloc(max_copy * sizeof(char));
        strcpy(s, "REAL ");
        sprintf(t, "%f", l->value.real);
        strncat(s, t, max_copy);
        free(t);
        return s;
    }
    case TRUE :
        strcpy(s, "true");
        return s;
    case FALSE :
        strcpy(s, "false");
        return s;
    case STRING :
    {
        strcpy(s, "STRING \"");
        strncat(s, l->value.string, max_copy);
        strcat(s, "\"");
        return s;
    }
    case OPEN_PAREN :
        strcpy(s, "OPEN_PAREN '('");
        return s;
    case CLOSE_PAREN :
        strcpy(s, "CLOSE_PAREN ')'");
        return s;
    case OPEN_BRACKET :
        strcpy(s, "OPEN_BRACKET '['");
        return s;
    case CLOSE_BRACKET :
        strcpy(s, "CLOSE_BRACKET ']'");
        return s;
    case OPEN_BRACE :
        strcpy(s, "OPEN_BRACE '{'");
        return s;
    case CLOSE_BRACE :
        strcpy(s, "CLOSE_BRACE '}'");
        return s;
    case VERTICAL_BAR :
        strcpy(s, "VERTICAL_BAR '|'");
        return s;
    case COMMA :
        strcpy(s, "COMMA ','");
        return s;
    case SEMICOLON :
        strcpy(s, "SEMICOLON ';'");
        return s;
    case COLON :
        strcpy(s, "COLON ':'");
        return s;
    case PLUS :
        strcpy(s, "PLUS '+'");
        return s;
    case MINUS :
        strcpy(s, "MINUS '-'");
        return s;
    case TIMES :
        strcpy(s, "TIMES '*'");
        return s;
    case DIVIDED_BY :
        strcpy(s, "DIVIDED_BY '/'");
        return s;
    case EXPONENT :
        strcpy(s, "EXPONENT '^'");
        return s;
    case REMAINDER :
        strcpy(s, "REMAINDER '%'");
        return s;
    case MOD :
        strcpy(s, "MOD 'mod'");
        return s;
    case EQUALS :
        strcpy(s, "EQUALS '=' or 'equals'");
        return s;
    case NOT_EQUALS :
        strcpy(s, "NOT_EQUALS '~=' or 'not equals'");
        return s;
    case LESS_THAN :
        strcpy(s, "LESS_THAN '<'");
        return s;
    case GREATER_THAN :
        strcpy(s, "GREATER_THAN '>'");
        return s;
    case LESS_THAN_OR_EQUALS :
        strcpy(s, "LESS_THAN_OR_EQUALS '<='");
        return s;
    case GREATER_THAN_OR_EQUALS :
        strcpy(s, "GREATER_THAN_OR_EQUALS '>='");
        return s;
    case NOT :
        strcpy(s, "NOT '~'");
        return s;
    case AND :
        strcpy(s, "AND 'and'");
        return s;
    case OR :
        strcpy(s, "OR 'or'");
        return s;
    case IF :
        strcpy(s, "IF 'if'");
        return s;
    case QUESTION_MARK :
        strcpy(s, "QUESTION_MARK '?'");
        return s;
    case ELSE :
        strcpy(s, "ELSE 'else'");
        return s;
    case WHILE :
        strcpy(s, "WHILE 'while'");
        return s;
    case UNKNOWN :
        strcpy(s, "UNKNOWN");
        return s;
    case END_OF_INPUT :
        strcpy(s, "END_OF_INPUT");
        return s;
    case LINK :
        strcpy(s, "LINK");
        return s;
    case NIL :
        strcpy(s, "NIL");
        return s;
    case UNINITIALIZED :
        strcpy(s, "UNINITIALIZED");
        return s;
    case FUNCTION_CALL :
        strcpy(s, "FUNCTION_CALL");
        return s;
    case UNARY_PLUS :
        strcpy(s, "UNARY_PLUS");
        return s;
    case UNARY_MINUS :
        strcpy(s, "UNARY_MINUS");
        return s;
    case ARRAY_ACCESS :
        strcpy(s, "ARRAY_ACCESS");
        return s;
    case PARENTHESIS :
        strcpy(s, "PARENTHESIS");
        return s;
    case CURLY_BRACES :
        strcpy(s, "CURLY_BRACES");
        return s;
    case CASE :
        strcpy(s, "CASE");
        return s;
    case WHILE_SPLIT :
        strcpy(s, "WHILE_SPLIT");
        return s;
    case ARRAY :
        strcpy(s, "ARRAY");
        return s;
    case LAMBDA :
        strcpy(s, "LAMBDA");
        return s;
    case CLOSURE :
        strcpy(s, "CLOSURE");
        return s;
    case BODY:
        strcpy(s, "BODY");
        return s;
    case ENV :
        strcpy(s, "ENV");
        return s;
    case TABLE :
        strcpy(s, "TABLE");
        return s;
    case THIS:
        strcpy(s, "THIS");
        return s;
    case RETURN:
        strcpy(s, "RETURN");
        return s;
    case BUILTIN: 
        strcpy(s,"BUILTIN");
        return s;
    default :
        printf("Unexpected type: %d\n", l->type);
        exit(EXIT_FAILURE);
    }
}



