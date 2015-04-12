%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "envar.h"

%}

%option stack
%x quotedword envExpansion
%%

[0-9]+    				yylval.number = atoi(yytext); return NUMBER;
cd 						yylval.string = strdup(yytext);return TOKCD;
setenv 					yylval.string = strdup(yytext);return TOKSETENV;
unsetenv				yylval.string = strdup(yytext);return TOKCLEARENV;
printenv				yylval.string = strdup(yytext);return TOKPRINTENV;
alias 					yylval.string = strdup(yytext);return TOKALIAS;
unalias 				yylval.string = strdup(yytext);return TOKUNALIAS;
bye 					yylval.string = strdup(yytext);return TOKBYE;

[^<>|\&" \t\n$]+		yylval.string = strdup(yytext);return WORD;


\"						yy_push_state(quotedword);
<quotedword>{
	[^"\n\{\}]+      	yylval.string = strdup(yytext);return WORD;
	\"					yy_pop_state();
	\n 					printf(":p \">");
	[\n\{\}]			/* ignore certain metacharacters*/
}

<*>\$\{					yy_push_state(envExpansion);
<envExpansion>{
	[^<>|\&" \t\n\{\}]+ yylval.string = getenv(yytext);return WORD;
	\}					yy_pop_state();
	\"					yy_push_state(quotedword);
	[<>|\& \t\n]		/* ignore certain metacharacters*/
}

\| 						return TOKPIPE;
\<						return TOK_IO_REDIR_IN;
\> 						return TOK_IO_REDIR_OUT;
\>\>					return TOK_IO_REDIR_OUT_APPEND;
\n          			return TOKENDEXP;
[ \t]+      		/* ignore whitespace */;
%%

void flush_buffer(){
	yy_flush_buffer(YY_CURRENT_BUFFER);
}


void scan_string(char* str){
	//to accomodate the yacc grammar 
	//we copied the string and added a newline character 
	char* new_str;
	new_str = strdup(str);
	strcat(new_str, "\n");
	
	/*FILE *f = fopen("file.txt", "w+");
	if (f == NULL)
	{
	    printf("Error scanning alias!\n");
	    return;
	}

	fprintf(f, "%s", new_str);*/

	YY_BUFFER_STATE old = YY_CURRENT_BUFFER;
	YY_BUFFER_STATE buf = yy_scan_string(new_str);
	yyparse();
	yy_flush_buffer(buf);
	yylex_destroy();
	yy_switch_to_buffer(old);
	//fclose(f);
}
