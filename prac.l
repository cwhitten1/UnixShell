%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

int inQuoteMode = 0;
%}

%x quotedword
%%

[0-9]+    				yylval.number=atoi(yytext); return NUMBER;
cd 						yylval.string = strdup(yytext);return TOKCD;
setenv 					yylval.string = strdup(yytext);return TOKSETENV;
unsetenv				yylval.string = strdup(yytext);return TOKCLEARENV;
printenv				yylval.string = strdup(yytext);return TOKPRINTENV;
alias 					yylval.string = strdup(yytext);return TOKALIAS;
unalias 				yylval.string = strdup(yytext);return TOKUNALIAS;
bye 					yylval.string = strdup(yytext);return TOKBYE;

[^<>|\&" \t\n]+			yylval.string = strdup(yytext);return WORD;


<quotedword>[^<>|"\n\&]+      		yylval.string = strdup(yytext);return WORD;
\"						BEGIN(quotedword);
<quotedword>\"			BEGIN(INITIAL);
<quotedword>\n 			BEGIN(INITIAL);


\n          			return TOKNEWLINE;
[ \t]+      		/* ignore whitespace */;
%%