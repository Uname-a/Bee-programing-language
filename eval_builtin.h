#ifndef EVAL_BUILTIN_H
#define EVAL_BUILTIN_H
#include "type.h"
#include "lexer.h"
#include "lexeme.h"
extern int countCL;
extern char **argsCL;
//print
Lexeme *evalPrint(Lexeme *arglist);
Lexeme *evalPrintln(Lexeme *arglist);
//force things to other stuff
Lexeme *evalAllocate(Lexeme *arglist);
Lexeme *evalReal(Lexeme *arglist);
Lexeme *evalInteger(Lexeme *arglist);
//list stuff
Lexeme *evalMakeList(Lexeme *arglist);
Lexeme *evalCons(Lexeme *arglist);
Lexeme *evalCar(Lexeme *arglist);
Lexeme *evalCdr(Lexeme *arglist);

Lexeme *evalSizeof(Lexeme *arglist);
//what type is it?
Lexeme *evalIsNil(Lexeme *arglist);
Lexeme *evalIsString(Lexeme *arglist);
Lexeme *evalIsInteger(Lexeme *arglist);
Lexeme *evalIsReal(Lexeme *arglist);
//command line
Lexeme *evalGetArg(Lexeme *arglist);
Lexeme *evalGetArgCount();
//file 
Lexeme *evalOpenFileForReading(Lexeme *arglist);
Lexeme *evalReadInteger(Lexeme *arglist);
Lexeme *evalAtFileEnd(Lexeme *arglist);
Lexeme *evalCloseFile(Lexeme *arglist);
#endif
