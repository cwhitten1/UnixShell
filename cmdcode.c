#include <stdbool.h>
#include <string.h>
#include "cmdcode.h"
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
    int c = 0;
    int d = 0;

    bool match1 = false;
    bool match2 = false;

    for(c; c < alias_count; c++){
        if(strcmp(alias, alias_table[c].cmd) == 0)
            match1 = true;
        
        for(d; d < alias_count; d++){
            if(strcmp(cmd, alias_table[d].ali) == 0)
                match2 = true;
        }

        if(match1 && match2)
            return -1;
    }

    return 0;
}


void executeOtherCommand(char* cmd_name){
    char* exec_path = searchPathForFile(cmd_name);
    if(exec_path == NULL)
        printf("\tCould not find \"%s\" on the PATH\n", cmd_name);
    else
    {
        printf("\tFound command %s!", cmd_name);
    }
}