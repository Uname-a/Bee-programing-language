// mgbaird
// some functions are based on reading material provided by Dr. Lusth
#include <stdio.h>
#include <unistd.h>
#include "evaluate.h"

int countCL;
char **argsCL;

int main(int argc, char* argv[])
{

	countCL = argc;
    argsCL = argv;
	char *filename = argv[1];
	Lexeme *parse_tree = parseProgram(filename);
	Lexeme *hidden_env = createEnv();

    initBuiltins(hidden_env);

    Lexeme *global_env = extendEnv(hidden_env,nilLexeme(),nilLexeme()); 

    eval(parse_tree,global_env);
	return 0;
}