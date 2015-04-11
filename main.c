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

void resetCommandTable(){
	cmdtab_curr = 0;
	cmdtab_start = 0;
	cmdtab_end = 0;
	num_commands = 0;
	clearCommandTable();
}

void shell_init(){ 
	//Initialize global vars
	exitRequested = 0;
	PATH = getenv("PATH");
	HOME = getenv("HOME");
	PWD = getenv("PWD");

	resetCommandTable();
}

void printCommandTable(){
	int i;
	printf("\n\n\nCurrent ind: %d\n", cmdtab_curr);
	for(i=0;i<MAX_COMMANDS;i++)
	{
			struct command cmd = commands[i];
			printf("%d: %s | %d | %d | %s | %s\n", i, cmd.name, cmd.id, cmd.argnum, cmd.infile, cmd.outfile);

	}
	printf("\n\n");

}

void handleAlias(int aliasIndex, int tableIndex){
	char* cmd = get_alias_cmd(aliasIndex);

	//Save old state
	int old_num_cmd = num_commands;

	//Remove the alias command (we will be inserting into this location)
	removeCommand(tableIndex);

	cmdtab_curr = tableIndex;      
    scan_string(cmd);
    int num_added = num_commands - old_num_cmd;
    cmdtab_end += num_added;

    
}

void expandAliases()
{
	int aliasInd;
	int cmd_ind = cmdtab_start;
	while(cmd_ind <= cmdtab_end)
	{
		struct command cmd = commands[cmd_ind];
		aliasInd = is_alias(cmd.name);
		if(aliasInd != -1)
		{
			printf("alias cmd ind: %d", cmd_ind);
			handleAlias(aliasInd, cmd_ind);
		}

		cmd_ind++;
	}
}

void handleCommand(){

	expandAliases(); //Expand any aliases in the table so we have either builtins or other commands in the table
	int cmd_ind = cmdtab_start;
	while(cmd_ind <= cmdtab_end)
	{
		struct command cmd = commands[cmd_ind];
		switch(cmd.id){
			case CD: change_dir(cmd.args[0]); break;
			case SETENV: set_env(cmd.args[0], cmd.args[1], 1);break;
			case UNSETENV: unset_env(cmd.args[0], 1);break;
			case PRINTENV: print_env();break;
			case SHOW_ALIAS: show_aliases();break;
			case SET_ALIAS: set_alias(cmd.args[0], cmd.args[1]);break;
			case UNSET_ALIAS: unset_alias(cmd.args[0]);
			case BYE: printf("\tBye!"); exitRequested = 1;break;
			case OTHER: executeOtherCommand(cmd.name);
				
			default: break;
		}
		cmd_ind++;
	}
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
	    	resetCommandTable();
	    }
	}

}



