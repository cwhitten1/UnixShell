%{
#include <stdio.h>
#include <string.h>
//int yydebug=1;

void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 


%}

%token TOKHEAT STATE TOKTARGET TOKTEMPERATURE TOKCD TOKCD_HOME
%union 
{
        int number;
        char *string;
}

%token <number> NUMBER
%token <string> WORD

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