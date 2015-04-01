/* 
 * This file is used to define the 'command' struct which will be used to store commands which are input by the user.
 */
#include <string.h>
#define MAX_ARGS 300
#define MAX_COMMANDS 100

struct command{
	char* 	name;
	char* 	args[MAX_ARGS];
	int 	argnum;
	char*	infile;
	char*	outfile;
};

extern struct command commands[];
extern int cmdtab_next;