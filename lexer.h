// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexeme.h"


char skipWhitespace(FILE *fp);
Lexeme *lexColonOrDefineOrEnddefine(FILE *fp);
Lexeme *lexEqualsOrAssign(FILE *fp);
Lexeme *lexGreaterThan(FILE *fp);
Lexeme *lexLessThan(FILE *fp);
Lexeme *lexNotOrNotEquals(FILE *fp);
Lexeme *lexNumber(FILE *fp);
Lexeme *lexString(FILE *fp);
Lexeme *lexVariableOrKeyword(FILE *fp);
FILE *lexFopen(char *filename);
int getNumber();
Lexeme *lex(FILE *fp);
#endif
