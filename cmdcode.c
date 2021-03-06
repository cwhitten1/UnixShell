#include <stdbool.h>
#include <string.h>
#include "cmdcode.h"
#include "command.h"
#include "alias.h"



struct alias alias_table[MAX_SIZE];
int alias_count = 0;

void print_env(){
	int i=0;

    while(environ[i]){
      printf("%s\n\n", environ[i++]);
    }

	printf("\n\n\n\n");
}

void set_env(char* env_var, char* val, int printOutput){

    //Copy new value to local variable (if one exists i.e. PATH)
    char* env_var_local = getLocalEnvironmentVariable(env_var);
    if(env_var_local != NULL)
	   strcpy(env_var_local, val);

    //Modify process environment variable
    int success = setenv(env_var, val, 1); //Returns 0 on success, -1 otherwise

    if(printOutput)
    {
        if(success != 0)
                printf("\tVariable %s not found", env_var);
        else
        {
                 printf("\tSet variable %s to %s\n", env_var, val); 
        }
    }
}

void unset_env(char* env_var, int printOutput){
    //Remove local copy
	char* env_var_local = getLocalEnvironmentVariable(env_var);
    if(env_var_local != NULL)
       env_var_local = NULL;

    //Modify process environment variable
    int success = unsetenv(env_var); //Returns 0 on success, -1 otherwise
    if(printOutput)
    {
        if(success != 0)
                printf("\tVariable %s not found", env_var);
        else
        {
                 printf("\tCleared variable %s\n", env_var);
        }
    }
}

void change_dir(char* word){
	char* cwd = malloc(100);
    int success = chdir(word);
    getcwd(cwd, 100);
    if(success == 0)
    {
    	printf("\tChanged directory to %s\n", cwd);
    	set_env("PWD", cwd, 0);
    }
    else
    	printf("\tDirectory \"%s\" not found\n", word);

    free(cwd);       
}

/********************************
        ALIAS FUNCTIONS
*********************************/

void set_alias(char* name, char* word){
    int i = 0;


    //check for alias duplicates
    if(alias_count > 0){
        for(i; i < alias_count; ++i){
            if(strcmp(name, alias_table[i].ali) == 0){
                printf("\t%s already exists.\n\tPlease use a different name.\n", name);
                return;
            }
        }

        //check if name, word combination creates an infinite recursion
        int finite = check_infinite_alias(name, word);
        //infinite recursion detected
        if(finite == -1){
            printf("\tInfinite alias detected\n");
            return;
        }
    }

    //insert alias name and command into alias table
    alias_table[alias_count].ali = name;
    alias_table[alias_count].cmd = word;
    alias_count++;

    printf("\tAlias %s set as %s\n", name, word);
}

void show_aliases(){
    if(alias_count == 0){
        printf("\tThere are no aliases\n");
    }
    else{
        int i = 0;

        //iterate through alias table and print
        while(i != alias_count)
        {
            char* name = alias_table[i].ali;
            char* word = alias_table[i].cmd;

            ++i;

            printf("\tAlias: %s \tCommand: %s\n", name, word);
        }
    }
}

void unset_alias(char* name){
    int i = 0;
    bool found = false;

    if(alias_count == 0){
        printf("\tThere are not aliases\n");
        return;
    }

    //find alias with specified name and overwrite with n+1 alias
    while(!found){
        if(strcmp(name, alias_table[i].ali) == 0){
            int c = i; 
            for(c; c <= alias_count-1; c++)
                alias_table[c] = alias_table[c+1];
            
            found = true;
            alias_count--;
        }
        ++i;
    }

    printf("\tCleared alias %s\n", name);
}

int is_alias(char* name){
    int i = 0;

    //check if WORD is an actual alias
    for(i; i < alias_count; ++i){
        //if match return its index
        if(strcmp(name, alias_table[i].ali) == 0){
            return i;
        }
            
    }
    return -1;
}

char* get_alias_cmd(int index){
    int j = index;
    char* command;
    command = strdup(alias_table[j].cmd);

    //check if command is also an alias
    int check = is_alias(command);

    //if not -1 then its still an alias
    if(check != -1)
        get_alias_cmd(check);
    else
        return command;
}

int check_infinite_alias(char* alias, char* cmd){
    int cmd_index = is_alias(cmd); //Does the cmd exist already in the table

    //If cmd doesn't exist yet, an infinite alias cannot happen
    if(cmd_index == -1)
        return 0;

    char* expanded_cmd = get_alias_cmd(cmd_index);

    //If expanded form of the command is the same as the new alias name, then this would create a circle
    //leading to an infinite alias
    if(strcmp(alias, expanded_cmd) == 0)
        return -1;
    else
        return 0;
}

//Returns 1 if command was executed successfully, 0 if cmd wasn't found, -1 otherwise
int executeOtherCommand(int cmd_ind){
    struct command cmd = commands[cmd_ind];
    char* cmd_name = cmd.name;
    char* exec_path = searchPathForFile(cmd.name);
    if(exec_path == NULL)
        return 0;
    else
    {
        int status; //Used for getting exit status of child process
        //printf("\tFound command %s!", cmd_name);
        pid_t pid = fork();

        //If forking failed
        if(pid == -1)
        {
            printf("\tFailed to fork new process\n");
            return -1;
        }
        //If child process is executing this
        else if(pid == 0)
        {
            //Setup redirection
            if(cmd.infile != NULL && strcmp(cmd.infile, "PIPE") != 0)
            {
                int redir_success = redirect_in(cmd.infile);
                if(redir_success != 0)
                    printf("\tFile %s not readable or does not exist.\n", cmd.infile);
            }
                
            if(cmd.outfile != NULL && strcmp(cmd.outfile, "PIPE") != 0)
            {
                int redir_success = redirect_out(cmd.outfile, cmd.appendOut);
                if(redir_success != 0)
                    printf("\tFailed to write to file %s.\n", cmd.outfile);
            }

            //Execute command
            int success = execv(exec_path, cmd.args);
            if(success == -1)
                exit(1);
            exit(0);
        }
        //Otherwise parent process is executing
        else
        {
            printf("\tExecuting...\n");
            wait(&status); //Wait for child process
            if(WEXITSTATUS(status) != 0)
                return -1;
            else
                return 1;
        }

    }
}