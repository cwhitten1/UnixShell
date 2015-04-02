%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}

%%
[0-9]+    				yylval.number=atoi(yytext); return NUMBER;
cd[ \t\r]+[a-zA-Z]		yyless(yyleng-1);return TOKCD;   /*Return the last character to be scanned as input*/
cd 						return TOKCD_HOME;
setenv 					return TOKSETENV;
unsetenv				return TOKCLEARENV;
printenv				return TOKPRINTENV;
heat					return TOKHEAT;
on|off 					return STATE;
target 					return TOKTARGET;
temperature 			return TOKTEMPERATURE;
[a-zA-Z]+       		yylval.string = strdup(yytext);return WORD;
\n          		/* ignore end of line */;
[ \t]+      		/* ignore whitespace */;
%%