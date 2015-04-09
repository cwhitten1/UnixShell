/* 
 * This file is used to define the 'command' struct which will be used to store commands which are input by the user.
 */
#include <string.h>
#define MAX_ARGS 30
#define MAX_COMMANDS 10

struct command{
	char* 	name;
	int 	cmd_id; /* Should be a constant defined in command.h*/
	char* 	args[MAX_ARGS];
	int 	argnum;
	char*	infile;
	char*	outfile;
};

extern struct command commands[];

//Table indexes
extern int cmdtab_curr; /* The current index for parsing a line of commands */
extern int cmdtab_start; /* The start index for a line of commands */
extern int cmdtab_end; /* The end index for a line of commands */