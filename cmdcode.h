#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "envar.h"
#include "path_handle.h"

extern char **environ;

#define CD 1
#define SETENV 2
#define UNSETENV 3
#define PRINTENV 4
#define SHOW_ALIAS 5
#define SET_ALIAS 6
#define UNSET_ALIAS 7
#define BYE 8
#define OTHER 9

void print_env();
void change_dir();
void set_env(char* env_var, char* val, int printOutput);
void unset_env(char* env_var, int printOutput);
void show_aliases();
void set_alias();
void unset_alias(char* name);
void executeOtherCommand(char* cmd_name); /* Handles execution of non-builtin commands*/

int is_alias(char* name);
char* get_alias_cmd(int index);
int check_infinite_alias(char* alias, char* cmd);