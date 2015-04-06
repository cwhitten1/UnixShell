%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "envar.h"

%}

%x quotedword envExpansion
%%

[0-9]+    				yylval.number=atoi(yytext); return NUMBER;
cd 						yylval.string = strdup(yytext);return TOKCD;
setenv 					yylval.string = strdup(yytext);return TOKSETENV;
unsetenv				yylval.string = strdup(yytext);return TOKCLEARENV;
printenv				yylval.string = strdup(yytext);return TOKPRINTENV;
alias 					yylval.string = strdup(yytext);return TOKALIAS;
unalias 				yylval.string = strdup(yytext);return TOKUNALIAS;
bye 					yylval.string = strdup(yytext);return TOKBYE;

[^<>|\&" \t\n$]+			yylval.string = strdup(yytext);return WORD;


\"						BEGIN(quotedword);
<quotedword>[^<>|"\n\&]+      		yylval.string = strdup(yytext);return WORD;
<quotedword>\"			BEGIN(INITIAL);
<quotedword>\n 			printf(":p \">");

\$\{						BEGIN(envExpansion);
<envExpansion>[^<>|\&" \t\n\{\}]+ 		yylval.string = getenv(yytext);return WORD;
<envExpansion>\}		BEGIN(INITIAL);
<envExpansion>[<>|\&" \t\n]		/* ignore certain metacharacters*/


\n          			return TOKNEWLINE;
[ \t]+      		/* ignore whitespace */;
%%