#include <stdio.h>
#include "y.tab.h"
#include "command.h"
#include "envar.h"

struct command commands[MAX_COMMANDS];
int exitRequested;

void printShellSymbol()
{
        printf(":p > ");
} 

void shell_init(){ 
	exitRequested = 0;
}

int main()
{
	shell_init();
	while(!exitRequested)
	{
	    printShellSymbol();
	    yyparse();
	}

}



