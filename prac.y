%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "envar.h"
#include "cmdcode.h"
#include "alias.h"

//int yydebug=1;

int cmdtab_next = 0;
char* invalid_cmd;

void yyerror(const char *str)
{
        flush_buffer();
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
}

%}


%token <string> TOKCD
%token <string> TOKSETENV TOKCLEARENV TOKPRINTENV
%token <string> TOKALIAS TOKUNALIAS
%token <string> TOKQUOTE
%token <string> TOKBYE 
%token TOKENDEXP TOKPIPE TOK_IO_REDIR_IN TOK_IO_REDIR_OUT

%union 
{
        int number;
        char *string;
}

%token <number> NUMBER
%token <string> WORD
%token <string> TOKHEAT

%%
commands: /* empty */
        | 
        commands command
        ;

command:
        change_dir
        |
        change_dir_home
        |
        set_env_var
        |
        unset_env_var
        |
        print_env_var
        |
        show_aliases
        |
        set_alias
        |
        unset_alias
        |
        bye
        |
        end_exp
        |
        quote
        |
        default
        |
        io_redir
        ;

change_dir:
        TOKCD WORD
        {
                change_dir($2);
                YYACCEPT;
        }

change_dir_home:
        TOKCD TOKENDEXP
        {
                change_dir(HOME);
                YYACCEPT;
        }
        ;

set_env_var:
        TOKSETENV WORD WORD
        {
                char* env_var = $2;
                if(env_var == NULL)
                        printf("\tVariable %s not found", $2);
                else
                {
                         printf("\tSet variable %s to %s\n", env_var, $3);
                         set_env(env_var, $3);   
                }
                YYACCEPT;
        }
        ;

unset_env_var:
        TOKCLEARENV WORD
        {
                char* env_var = $2;
                if(env_var == NULL)
                        printf("\tVariable %s not found", $2);
                else
                {
                         printf("\tCleared variable %s\n", env_var);
                         unset_env(env_var);   
                }
                YYACCEPT;
        }
        ;

print_env_var:
        TOKPRINTENV
        {
                print_env();
                YYACCEPT;
        }
        ;

show_aliases:
        TOKALIAS
        {
                show_aliases();
                YYACCEPT;  
        }
        ;

//This rule causes a shift/reduce conflict but I don't know why
set_alias:
        TOKALIAS WORD WORD
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        |
        TOKALIAS WORD TOKCD
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKSETENV
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKCLEARENV
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKPRINTENV
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKALIAS
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKUNALIAS
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        TOKALIAS WORD TOKBYE
        {
                set_alias($2, $3);
                YYACCEPT;
        }
        ;

unset_alias:
        TOKUNALIAS WORD
        {
                unset_alias($2);
                YYACCEPT;
        }
        ;

bye:
        TOKBYE
        {
                printf("\tBye!");
                exitRequested = 1;
                YYACCEPT;
        }

end_exp:
        TOKENDEXP
        {
        }
        ;
quote:
        TOKQUOTE
        {
        }
        ;

//This rule causes a shift/reduce conflict but I don't know why
default:
        default TOKENDEXP
        {
                printf("\tCommand \"%s\" not recognized\n", invalid_cmd);
                invalid_cmd = NULL;
                YYACCEPT;
        }
        |
        WORD
        {
                if(invalid_cmd == NULL)
                        invalid_cmd = $1;
        }
        ;

io_redir:
        TOK_IO_REDIR_IN WORD
        {
                printf("Redirecting input to %s", $2);
        }
        |
        TOK_IO_REDIR_OUT WORD
        {
                printf("Redirecting output to %s", $2);
        }
        ;
        
%%