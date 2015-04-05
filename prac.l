%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}

%%
[0-9]+    				yylval.number=atoi(yytext); return NUMBER;
cd[ \t\r]+[a-zA-Z]		yylval.string = strdup(yytext);yyless(yyleng-1);return TOKCD;   /*Return the last character to be scanned as input*/
cd 						yylval.string = strdup(yytext);return TOKCD_HOME;
setenv 					yylval.string = strdup(yytext);return TOKSETENV;
unsetenv				yylval.string = strdup(yytext);return TOKCLEARENV;
printenv				yylval.string = strdup(yytext);return TOKPRINTENV;
alias 					yylval.string = strdup(yytext);return TOKALIAS;
unalias 				yylval.string = strdup(yytext);return TOKUNALIAS;
bye 					yylval.string = strdup(yytext);return TOKBYE;


[a-zA-Z]+       		yylval.string = strdup(yytext);return WORD;
\n          			return TOKNEWLINE;
[ \t]+      		/* ignore whitespace */;
%%