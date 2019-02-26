// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#include "lexer.h"
#include "type.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
// set line number
static int line_number = 1;
// lexer
FILE *lexFopen(char *filename) {
  line_number = 1;
  initializeJustTypeLexemes();
  return fopen(filename, "r");
}

int getNumber() { return line_number; }


Lexeme *lex(FILE *fp) {
  int ch;

  ch = skipWhitespace(fp);

  if (ch == EOF)
    return newLexeme(END_OF_INPUT);

  switch (ch) {
  case '(':
    return newLexeme(OPEN_PAREN);
  case ')':
    return newLexeme(CLOSE_PAREN);
  case '[':
    return newLexeme(OPEN_BRACKET);
  case ']':
    return newLexeme(CLOSE_BRACKET);
  case '{':
    return newLexeme(OPEN_BRACE);
  case '}':
    return newLexeme(CLOSE_BRACE);
  case '|':
    return newLexeme(VERTICAL_BAR);
  case ',':
    return newLexeme(COMMA);
  case ';':
    return newLexeme(SEMICOLON);
  case '+':
    return newLexeme(PLUS);
  case '-':
    return newLexeme(MINUS);
  case '*':
    return newLexeme(TIMES);
  case '/':
    return newLexeme(DIVIDED_BY);
  case '^':
    return newLexeme(EXPONENT);
  case '%':
    return newLexeme(REMAINDER);
  case '?':
    return newLexeme(QUESTION_MARK);
  case '~':
    return lexNotOrNotEquals(fp);
  case ':':
    return lexColonOrDefineOrEnddefine(fp);
  case '=':
    return lexEqualsOrAssign(fp);
  case '<':
    return lexLessThan(fp);
  case '>':
    return lexGreaterThan(fp);

  default:
    if (isdigit(ch)) {
      ungetc(ch, fp);
      return lexNumber(fp);
    } else if (isalpha(ch)) {
      ungetc(ch, fp);
      return lexVariableOrKeyword(fp);
    } else if (ch == '\"') {
      return lexString(fp);
    }
    fprintf(stderr, "line %d: Unknown character \'%c\'.\n", line_number, ch);
    exit(EXIT_FAILURE);
  }
}

// other lex fuctions
char escapeChar(char ch) {
  switch (ch) {
  case 'n':
    return '\n';
  case 't':
    return '\t';
  case 'b':
    return '\b';
  case 'v':
    return '\v';
  case 'f':
    return '\f';
  case 'r':
    return '\r';
  case 'a':
    return '\a';
  default:
    return ch;
  }
}
Lexeme *lexNotOrNotEquals(FILE *fp) {
  int ch = fgetc(fp);
  if (ch == '=') {
    return newLexeme(NOT_EQUALS);
  }
  ungetc(ch, fp);
  return newLexeme(NOT);
}

Lexeme *lexColonOrDefineOrEnddefine(FILE *fp) {
  int ch = skipWhitespace(fp);
  if (ch == '=') {
    return newLexeme(DEFINE);
  }
  if (ch == ';') {
    return newLexeme(END_DEFINE);
  }
  ungetc(ch, fp);
  return newLexeme(COLON);
}

Lexeme *lexEqualsOrAssign(FILE *fp) {
  int ch = fgetc(fp);
  if (ch == '=') {
    return newLexeme(ASSIGN);
  }
  ungetc(ch, fp);
  return newLexeme(EQUALS);
}

Lexeme *lexLessThan(FILE *fp) {
  int ch = fgetc(fp);
  if (ch == '=') {
    return newLexeme(LESS_THAN_OR_EQUALS);
  }
  ungetc(ch, fp);
  return newLexeme(LESS_THAN);
}

Lexeme *lexGreaterThan(FILE *fp) {
  int ch = fgetc(fp);
  if (ch == '=') {
    return newLexeme(GREATER_THAN_OR_EQUALS);
  }
  ungetc(ch, fp);
  return newLexeme(GREATER_THAN);
}

Lexeme *lexNumber(FILE *fp) {
  int ch;
  int integer = 0;
  int digit;

  while (isdigit(ch = fgetc(fp))) {
    digit = ch - '0';
    integer = 10 * integer + digit;
  }

  if (ch == '.') // start read decimal digits
  {
    double real = (double)integer;
    integer = 0;
    int decimal_places = 0;

    while (isdigit(ch = fgetc(fp))) {
      digit = ch - '0';
      integer = 10 * integer + digit;
      ++decimal_places;
    }
    real += integer * pow(10, -decimal_places);

    ungetc(ch, fp);
    return newLexemeReal(REAL, real);
  }

  ungetc(ch, fp);
  return newLexemeInteger(INTEGER, integer);
}

Lexeme *lexVariableOrKeyword(FILE *fp) {
  char ch = fgetc(fp);

  int length = 0;
  int capacity = 2;
  char *string = malloc(capacity * sizeof(char));

  string[length++] = ch;

  while (isalnum(ch = fgetc(fp)) || ch == '_') {
    if (length == capacity)
    {
      capacity *= 2;
      string = realloc(string, capacity);
    }
    string[length++] = ch;
  }

  ungetc(ch, fp);

  if (length == capacity)
  {
    capacity += 1;
    string = realloc(string, capacity);
  }
  string[length] = '\0';


  if (strcmp(string, "true") == 0) {
    return newLexeme(TRUE);
  }
  else if (strcmp(string, "false") == 0) {
    return newLexeme(FALSE);
  }
  else if (strcmp(string, "mod") == 0) {
    return newLexeme(MOD);
  }
  else if (strcmp(string, "equals") == 0) {
    return newLexeme(EQUALS);
  }
  else if (strcmp(string, "not") == 0) {
    return newLexeme(NOT);
  }
  else if (strcmp(string, "and") == 0) {
    return newLexeme(AND);
  }
  else if (strcmp(string, "or") == 0) {
    return newLexeme(OR);
  }
  else if (strcmp(string, "if") == 0) {
    return newLexeme(IF);
  }
  else if (strcmp(string, "else") == 0) {
    return newLexeme(ELSE);
  }
  else if (strcmp(string, "while") == 0) {
    return newLexeme(WHILE);
  }
  else if (strcmp(string, "lambda") == 0) {
    return newLexeme(LAMBDA);
  }
  else if (strcmp(string, "nil") == 0) {
    return newLexeme(NIL);
  }
  else if (strcmp(string, "this") == 0) {
    return newLexeme(THIS);
  }
  else if (strcmp(string, "return") == 0) {
    return newLexeme(RETURN);
  }
  else {
    return newLexemeString(VARIABLE, string);
  }
}



Lexeme *lexString(FILE *fp) {
  char ch = fgetc(fp); // get one char

  int escape = 0;
  // check if escape character
  if (ch == '\\') {
    escape = 1;
    ch = fgetc(fp);
    ch = escapeChar(ch);
  }

  int length = 0;
  int capacity = 2;
  char *string = malloc(capacity * sizeof(char)); //size of 2

  string[length++] = ch;
  escape = 0;

  while (((ch = fgetc(fp)) != EOF) && ((ch != '"') || (escape == 1))) {
    if (ch == '\n') {
      ++line_number;
    }

    if (escape == 1) {
      ch = escapeChar(ch);
    }

    if ((escape == 0) && (ch == '\\')) {
      escape = 1;
    }
    else {
      if (length == capacity) {
        capacity *= 2;
        string = realloc(string, capacity);
      }
      string[length++] = ch;
      escape = 0;
    }
  }

  if (length == capacity) {
    capacity += 1;
    string = realloc(string, capacity);
  }
  string[length] = '\0';

  return newLexemeString(STRING, string);
}
char skipWhitespace(FILE *fp) { //comments start with #
  int ch;
  int comment = 0;
  // if ## then ignore until newline
  int superable = 0;

  while (((ch = fgetc(fp)) != EOF) &&
         (isspace(ch) || (comment > 0) || (ch == '#'))) {
    if (ch == '\n') {
      ++line_number;
    }

    switch (comment) {
    case 0:
      if (ch == '\n') {
        superable = 0;
      } else if (ch == '#') {
        if (superable == 1) {
          comment = 2;
        }
        else {
          superable = 1;
          comment = 1;
        }
      }
      break;
    case 1:
      if (ch == '\n') {
        comment = 0;
        superable = 0;
      } else if (ch == '#') {
        if (superable == 1) {
          comment = 2;
        } else {
          comment = 0;
          superable = 1;
        }
      } else {
        superable = 0;
      }
      break;
    case 2:
      // ignore all characters until newline
      if (ch == '\n') {
        comment = 0;
        superable = 0;
      }
      break;
    default:
      break;
    }
  }
  return ch;
}
