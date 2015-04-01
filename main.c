#include <stdio.h>
#include "y.tab.h"

void printShellSymbol()
{
        printf(":p > ");
} 

int main()
{
	while(1)
	{
	    printShellSymbol();
	    yyparse();
	}

}

