#include <stdbool.h>
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

void set_alias(char* name, char* word){
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

    for(i; i <= alias_count-1; ++i){
        if(strcmp(name, alias_table[i].ali) == 0)
            return 1;
    }
}

char* get_alias_cmd(char* name){
    int i = 0;

    for(i; i <= alias_count-1; ++i){
        if(strcmp(name, alias_table[i].ali) == 0)
            return alias_table[i].cmd;
    }
}