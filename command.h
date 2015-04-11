/* 
 * This file is used to define the 'command' struct which will be used to store commands which are input by the user.
 */
#include <string.h>
#include <stdarg.h>
#define MAX_ARGS 30
#define MAX_COMMANDS 10

struct command{
	char* 	name;
	int 	id; /* Should be a constant defined in command.h*/
	char* 	args[MAX_ARGS];
	int 	argnum;
	char*	infile;
	char*	outfile;
	int 	appendOut;
};

extern struct command commands[];

//Table indexes
extern int cmdtab_curr; /* The current index for parsing a line of commands */
extern int cmdtab_start; /* The start index for a line of commands */
extern int cmdtab_end;
extern int num_commands;

void addCommand(int ind, char* name, int id, int argnum, ...);
void insertCommand(int ind, char* name, int id, int argnum, ...);
void addArgToCommand(int cmd_ind, char* arg);
void addInputRedirection(int cmd_index, char* in_fn);
void addOutputRedirection(int cmd_index, char* out_fn, int appendOut);
void clearCommandTable();
void clearCommandArgs(struct command* cmd);