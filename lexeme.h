// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#ifndef LEXEME_H
#define LEXEME_H

#include <stdio.h>
#include "type.h"

typedef struct lexeme// lexeme structure
{
    int line_number;
    Type type;
    int array_size;
    union
    {
        char *string;
        char character;
        int integer;
        double real;
        FILE *fval;
        struct lexeme **array;
        struct lexeme *(*fp)(struct lexeme *);
    } value;
    struct lexeme *left;
    struct lexeme *right;
} Lexeme;
char *lexemeInfo(Lexeme *l);
char *lexemeInfoHelper(Lexeme *l);
char escapeChar(char ch);
Lexeme **just_type_Lexemes;
Lexeme *falseLexeme(void);
Lexeme *newJustTypeLexeme(Type type);
Lexeme *newLexeme(Type type);
Lexeme *newLexemeInteger(Type type, int integer);
Lexeme *newLexemeReal(Type type, double real);
Lexeme *newLexemeString(Type type, char *string);
Lexeme *nilLexeme(void);
Lexeme *trueLexeme(void);
void displayLexemeValue(Lexeme *l);
void initializeJustTypeLexemes(void);
#endif
