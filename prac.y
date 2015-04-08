%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "envar.h"
#include "cmdcode.h"
#include "alias.h"
#include "io_redir.h"


//int yydebug=1;

int cmdtab_next = 0;
char* first_cmd;

void yyerror(const char *str)
{
        flush_buffer();
        fprintf(stderr,"\terror: %s\n",str);
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

%start line

%%
line: /* empty */ 
        |
        commands TOKENDEXP {YYACCEPT;}
        |
        commands io_redir TOKENDEXP {YYACCEPT;}
        |
        default TOKENDEXP{
                if(is_alias(first_cmd) == 1){

                    scan_string(get_alias_cmd(first_cmd));
                }
                else
                    printf("\tCommand %s not recognized\n", first_cmd);

                first_cmd = NULL;
                YYACCEPT;
                }
        ;

commands: 
        command
        | 
        commands TOKPIPE command
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
        ;

change_dir:
        TOKCD WORD
        {
                change_dir($2);
                
        }

change_dir_home:
        TOKCD
        {
                change_dir(HOME);
                
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
                
        }
        ;

print_env_var:
        TOKPRINTENV
        {
                print_env();
                
        }
        ;

show_aliases:
        TOKALIAS
        {
                show_aliases();
                  
        }
        ;

set_alias:
        TOKALIAS WORD WORD
        {
                set_alias($2, $3);
                
        }
        |
        TOKALIAS WORD TOKCD
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKSETENV
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKCLEARENV
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKPRINTENV
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKALIAS
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKUNALIAS
        {
                set_alias($2, $3);
                
        }
        TOKALIAS WORD TOKBYE
        {
                set_alias($2, $3);
                
        }
        ;

unset_alias:
        TOKUNALIAS WORD
        {
                unset_alias($2);
                
        }
        ;

bye:
        TOKBYE
        {
                printf("\tBye!");
                exitRequested = 1;
                
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
io_redir:
        TOK_IO_REDIR_IN WORD
        {
                redirect_in($2);
        }
        |
        TOK_IO_REDIR_OUT WORD
        {
                printf("Redirecting output to %s", $2);
        }
        ;

default:
        default WORD
        |
        WORD
        {
                if(first_cmd == NULL)
                        first_cmd = $1;
        }
        ;
        
%%