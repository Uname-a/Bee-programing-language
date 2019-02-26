//mgbaird
#include "type.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char *typeToString(Type type)
{
    switch (type)
    {
    case AND : return "AND";
    case ARRAY : return "ARRAY";
    case ARRAY_ACCESS : return "ARRAY_ACCESS";
    case ASSIGN : return "ASSIGN";
    case BODY: return "BODY";
    case CASE : return "CASE";
    case CLOSE_BRACE : return "CLOSE_BRACE";
    case CLOSE_BRACKET : return "CLOSE_BRACKET";
    case CLOSE_PAREN : return "CLOSE_PAREN";
    case CLOSURE : return "CLOSURE";
    case COLON : return "COLON";
    case COMMA : return "COMMA";
    case CURLY_BRACES : return "CURLY_BRACES";
    case DEFINE : return "DEFINE";
    case DIVIDED_BY : return "DIVIDED_BY";
    case ELSE : return "ELSE";
    case END_DEFINE : return "END_DEFINE";
    case END_OF_INPUT : return "END_OF_INPUT";
    case ENV : return "ENV";
    case EQUALS : return "EQUALS";
    case EXPONENT : return "EXPONENT";
    case FALSE : return "FALSE";
    case FUNCTION_CALL : return "FUNCTION_CALL";
    case GREATER_THAN : return "GREATER_THAN";
    case GREATER_THAN_OR_EQUALS : return "GREATER_THAN_OR_EQUALS";
    case IF : return "IF";
    case INTEGER : return "INTEGER";
    case LAMBDA : return "LAMBDA";
    case LESS_THAN : return "LESS_THAN";
    case LESS_THAN_OR_EQUALS : return "LESS_THAN_OR_EQUALS";
    case LINK : return "LINK";
    case MINUS : return "MINUS";
    case MOD : return "MOD";
    case NIL : return "NIL";
    case NOT : return "NOT";
    case NOT_EQUALS : return "NOT_EQUALS";
    case OPEN_BRACE : return "OPEN_BRACE";
    case OPEN_BRACKET : return "OPEN_BRACKET";
    case OPEN_PAREN : return "OPEN_PAREN";
    case OR : return "OR";
    case PARENTHESIS : return "PARENTHESIS";
    case PLUS : return "PLUS";
    case QUESTION_MARK : return "QUESTION_MARK";
    case REAL : return "REAL";
    case REMAINDER : return "REMAINDER";
    case RETURN: return "RETURN";
    case SEMICOLON : return "SEMICOLON";
    case STRING : return "STRING";
    case TABLE : return "TABLE";
    case THIS: return "THIS";
    case TIMES : return "TIMES";
    case TRUE : return "TRUE";
    case UNARY_MINUS : return "UNARY_MINUS";
    case UNARY_PLUS : return "UNARY_PLUS";
    case UNINITIALIZED : return "UNINITIALIZED";
    case UNKNOWN : return "UNKNOWN";
    case VARIABLE : return "VARIABLE";
    case VERTICAL_BAR : return "VERTICAL_BAR";
    case WHILE : return "WHILE";
    case WHILE_SPLIT : return "WHILE_SPLIT";
    case BUILTIN : return "BUILTIN";
    case FILE_POINTER : return "FILE_POINTER";
    default :
        printf("Unexpected type: %d\n", type);
        exit(EXIT_FAILURE);
    }
}
