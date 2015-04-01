#include <stdio.h>
#include "y.tab.h"
#include "command.h"

struct command commands[MAX_COMMANDS];

void printShellSymbol()
{
        printf(":p > ");
} 

void shell_init(){ 
}

int main()
{
	shell_init();
	while(1)
	{
	    printShellSymbol();
	    yyparse();
	}

}



