%{
#include <stdio.h>
#include <string.h>


void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
void printShellSymbol()
{
        printf(":p > ");
} 

main()
{
        printShellSymbol();
        yyparse();
}


%}

%token TOKHEAT STATE TOKTARGET TOKTEMPERATURE TOKCD WSPACE
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
        ;

heat_switch:
        TOKHEAT STATE
        {
                printf("\tHeat turned on or off\n");
                printShellSymbol();
        }
        ;

target_set:
        TOKTARGET TOKTEMPERATURE NUMBER
        {
                printf("\tTemperature set to\n");
                printShellSymbol();
        }
        ;
change_dir:
        TOKCD
        {
                printf("\tChanged directory to previous directory");
        }
        |
        TOKCD WORD
        {
                printf("\tChanged directory to %s\n", $2);
                printShellSymbol();
        }
        ;
%%