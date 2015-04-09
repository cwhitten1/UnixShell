#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "command.h"
#include "envar.h"
#include "alias.h"
#include "cmdcode.h"

struct command commands[MAX_COMMANDS];

void printShellSymbol()
{
        printf(":p > ");
} 

void shell_init(){ 
	//Initialize global vars
	exitRequested = 0;
	PATH = getenv("PATH");
	HOME = getenv("HOME");
	PWD = getenv("PWD");
}

void printCommandTable(){
	int i;
	printf("Current ind: %d\n", cmdtab_curr);
	for(i=0;i<MAX_COMMANDS;i++)
	{
			struct command cmd = commands[i];
			printf("%d: %s | %d | %d | %s | %s\n", i, cmd.name, cmd.cmd_id, cmd.argnum, cmd.infile, cmd.outfile);

	}

}
void handleCommand(){
	int cmd_ind = cmdtab_curr % MAX_COMMANDS;
	struct command cmd = commands[cmd_ind];
	switch(cmd.cmd_id){
		case CD: change_dir(cmd.args[0]); break;
		case SETENV: set_env(cmd.args[0], cmd.args[1], 1);break;
		case UNSETENV: unset_env(cmd.args[0], 1);break;
		case PRINTENV: print_env();break;
		case SHOW_ALIAS: show_aliases();break;
		case SET_ALIAS: set_alias(cmd.args[0], cmd.args[1]);break;
		case UNSET_ALIAS: unset_alias(cmd.args[0]);
		case BYE: printf("\tBye!"); exitRequested = 1;break;
		default: break;
	}

	cmdtab_curr++;
}

int main()
{
	shell_init();
	while(!exitRequested)
	{
	    printShellSymbol();
	    int success = yyparse();

	    if(success == 0)
	    {
	    	handleCommand();
	    	printCommandTable();
	    }
	}

}



