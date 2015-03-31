%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}

%%
[0-9]+    		yylval.number=atoi(yytext); return NUMBER;
cd 				return TOKCD;
heat			return TOKHEAT;
on|off 			return STATE;
target 			return TOKTARGET;
temperature 	return TOKTEMPERATURE;
[a-zA-Z]+       yylval.string = strdup(yytext);return WORD;
\n          /* ignore end of line */;
[ \t]+      /* ignore whitespace */;
%%