%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

int inQuoteMode = 0;
%}

%x quotedword
%%

[0-9]+    				yylval.number=atoi(yytext); return NUMBER;
cd 						return TOKCD;
setenv 					return TOKSETENV;
unsetenv				return TOKCLEARENV;
printenv				return TOKPRINTENV;
alias 					return TOKALIAS;
unalias 				return TOKUNALIAS;
bye 					return TOKBYE;

[^<>|\&" \t\n]+			yylval.string = strdup(yytext);return WORD;


<quotedword>[^<>|"\n\&]+      		yylval.string = strdup(yytext);return WORD;
\"						BEGIN(quotedword);
<quotedword>\"			BEGIN(INITIAL);
<quotedword>\n 			BEGIN(INITIAL);


\n          			return TOKNEWLINE;
[ \t]+      		/* ignore whitespace */;
%%