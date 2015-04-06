%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "envar.h"
#include "cmdcode.h"

//int yydebug=1;

int cmdtab_next = 0;
char* invalid_cmd;

void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 


%}

%token TOKCD TOKCD_HOME 
%token TOKSETENV TOKCLEARENV TOKPRINTENV
%token TOKALIAS TOKUNALIAS
%token TOKNEWLINE
%token TOKBYE

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
        | commands command
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
        newline
        |
        default
        ;

change_dir:
        TOKCD WORD
        {
                change_dir($2);
                YYACCEPT;
        }

change_dir_home:
        TOKCD_HOME
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
                printf("\tThis command will show all aliases\n");
                YYACCEPT;  
        }
        ;

//This rule causes a shift/reduce conflict but I don't know why
set_alias:
        TOKALIAS WORD WORD
        {
                printf("\tSet alias %s to %s\n", $2, $3);
                YYACCEPT;
        }
        ;

unset_alias:
        TOKUNALIAS WORD
        {
                printf("\tCleared alias %s\n", $2);
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

newline:
        TOKNEWLINE
        {
        }
        ;

//This rule causes a shift/reduce conflict but I don't know why
default:
        default TOKNEWLINE
        {
                printf("\tCommand %s not recognized\n", invalid_cmd);
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
       
%%