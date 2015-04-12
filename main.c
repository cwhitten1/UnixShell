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
			printf("%d: %s | %d | %d | %s | %s | %d\n", i, cmd.name, cmd.id, cmd.argnum, cmd.infile, cmd.outfile, cmd.appendOut);

	}
	printf("\n\n");

}

//Returns index of infraction if has bad redirection, -1 for invalid end value, -2 for no bad redirection
int cmdTabHasInvalidRedirection(int end)
{
	int start = 0;
	if(end > MAX_COMMANDS -1)
		return -1;
	
	//If first command has in pipe, set it to null because that is not allowed
	//This is a bit of a hack to combat the weird bug where 
	//Initial command has in pipe after alias expansion
	if(commands[start].infile != NULL && strcmp(commands[start].infile, "PIPE") == 0)
		commands[start].infile = NULL;

	//Check rest of commands for broken pipes
	for(start; start < end; start++)
	{
		struct command cmd_curr = commands[start];
		struct command cmd_next = commands[start+1];
		int currPipe = strcmp(cmd_curr.outfile, "PIPE");
		int nextPipe =  strcmp(cmd_next.infile, "PIPE");

		//If current command has input redirection and is builtin, redirection is invalid
		if(cmd_curr.id != OTHER && cmd_curr.infile != NULL)
			return start;

		//If in/out pipes don't match, pipeline is broken
		if((currPipe == 0 && nextPipe != 0) || (currPipe != 0 && nextPipe == 0) )
			return start;

		//If next command has in pipe and is builtin, pipeline is broken
		if(cmd_next.id != OTHER && nextPipe == 0)
			return start+1;

	}

	//If last command has out pipe it is invalid 
	if(commands[end].outfile != NULL && strcmp(commands[end].outfile, "PIPE") == 0)
		return end;
	
	//If table passes previous checks
	return -2;
}

//Returns # of rows added to table
//in_name and out_name are used to manually set the redirection instead of relying on the scan of cmd
//scanning cmd caused the io redirect paths to be incorrect so this is a bit of a hack.
int expandAlias(int aliasIndex, int tableIndex, char* in_name, char* out_name){
	char* cmd = get_alias_cmd(aliasIndex);

	//Save old state
	int old_num_cmd = num_commands;
	//Remove the alias command (we will be inserting into this location)
	removeCommand(tableIndex);

	//Scan the expanded alias
	cmdtab_curr = tableIndex;      
    scan_string(cmd);
    int num_added = num_commands - old_num_cmd;

    //Set the proper io redirection
    //Any redirection the alias has will be overwritten if passed in args
    //are not equal to null
    if(in_name != NULL)
		commands[tableIndex].infile = strdup(in_name);
	if(out_name != NULL)
    	commands[tableIndex + num_added].outfile = strdup(out_name);

    return num_added;

    
}

void expandAliases()
{
	int cmd_ind = cmdtab_start;

	//Iterate through the command table
	while(cmd_ind <= cmdtab_end)
	{
		int aliasInd;
		char* in_file; 
		char* out_file;

		struct command cmd = commands[cmd_ind];
		in_file = cmd.infile;
		out_file = cmd.outfile;
		aliasInd = is_alias(cmd.name);


		if(aliasInd != -1)
		{
			int old_tab_end = cmdtab_end; //Save old state
			int numAdded = expandAlias(aliasInd, cmd_ind, in_file, out_file);
			cmdtab_end = old_tab_end; //Restore cmdtab pointer
			cmdtab_end += numAdded; // Extend end pointer to account for new commands alias added
			cmd_ind += numAdded; //Skip over the new cmds we just added from expanded alias
		}

		cmd_ind++;
	}
}

void handleCommandLine(){

	//Expand any aliases in the table so we have either builtins or other commands in the table
	expandAliases(); 

	//Check for broken pipelines after expansion
	int hasBrokenPipeline = cmdTabHasInvalidRedirection(cmdtab_end); 
	if(hasBrokenPipeline >= 0)
	{
		printf("\nDetected broken pipeline:\nCommand at index %d has I/O redirection already or is a built-in command.\n", hasBrokenPipeline);
		return;
	}
	else if( hasBrokenPipeline == -1)
	{
		printf("My bad.\n");
		return;
	}

	//Handle execution of commands
	int cmd_ind = cmdtab_start;
	int flush_signaled = 0;
	while((cmd_ind <= cmdtab_end) && !flush_signaled)
	{
		int cmd_success;
		struct command cmd = commands[cmd_ind];
		switch(cmd.id){
			case CD: change_dir(cmd.args[0]); break;
			case SETENV: set_env(cmd.args[0], cmd.args[1], 1);break;
			case UNSETENV: unset_env(cmd.args[0], 1);break;
			case PRINTENV: print_env();break;
			case SHOW_ALIAS: show_aliases();break;
			case SET_ALIAS: set_alias(cmd.args[0], cmd.args[1]);break;
			case UNSET_ALIAS: unset_alias(cmd.args[0]);break;
			case BYE: printf("\tBye!"); exitRequested = 1;break;
			case OTHER: 
				cmd_success = executeOtherCommand(cmd_ind);
				if( cmd_success == -1)
				{
					printf("\tFailed to execute command %s. Command line flushed.\n", cmd.name);
					flush_signaled = 1;
				}
				else if ( cmd_success == 0)
				{
					printf("\tCould not find \"%s\" on the PATH\n", cmd.name);
					flush_signaled = 1;
				}
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
	    	handleCommandLine();
	    	printCommandTable();
	    }

	    resetCommandTable();
	}

	return 0;

}



