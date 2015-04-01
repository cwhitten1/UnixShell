#include <stdio.h>
#include "y.tab.h"

int main()
{
	while(1)
	{
	    printShellSymbol();
	    yyparse();
	}

}