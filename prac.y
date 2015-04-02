%{
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "envar.h"

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

%token STATE TOKTARGET TOKTEMPERATURE 
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
        heat_switch
        |
        target_set
        |
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

heat_switch:
        TOKHEAT
        {
                struct command *cmd_slot = &commands[cmdtab_next];
                cmd_slot->name = $1;
                cmd_slot->argnum = 0;
                cmd_slot->infile = "stdin";
                cmd_slot->outfile = "stdout";
                printf("\tHeat turned on or off\n");
                YYACCEPT;
        }
        ;

target_set:
        TOKTARGET TOKTEMPERATURE NUMBER
        {
                printf("\tTemperature set to %d\n", $3);
                YYACCEPT;
        }
        ;





change_dir:
        TOKCD WORD
        {
                printf("\tChanged directory to %s\n", $2);
                YYACCEPT;
        }

change_dir_home:
        TOKCD_HOME
        {
                printf("\tChanged directory to home directory\n");
                YYACCEPT;
        }
        ;

set_env_var:
        TOKSETENV WORD WORD
        {
                printf("\tSet variable %s to %s\n", $2, $3);
                YYACCEPT;
        }
        ;

unset_env_var:
        TOKCLEARENV WORD
        {
                printf("\tCleared variable %s\n", $2);
                YYACCEPT;
        }
        ;

print_env_var:
        TOKPRINTENV
        {
                printf("\tPrint function unavailable until env variables are coded.\n");
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