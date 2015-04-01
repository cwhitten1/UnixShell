%{
#include <stdio.h>
#include <string.h>
#include "command.h"
//int yydebug=1;

int cmdtab_next = 0;

void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 


%}

%token STATE TOKTARGET TOKTEMPERATURE TOKCD TOKCD_HOME
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
default:
        WORD
        {
                printf("\tUnknown command: %s \n", $1);
                YYACCEPT;
        }
%%