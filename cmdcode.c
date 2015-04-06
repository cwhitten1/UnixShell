#include <stdbool.h>
#include "cmdcode.h"
#include "alias.h"

struct alias alias_table[MAX_SIZE];
int alias_count = 0;

void print_env(){
	printf("PATH: %s\n", PATH);
	printf("HOME: %s\n", HOME);
	printf("PWD: %s\n", PWD);
	printf("\n\n\n\n");
}

//The first arg should be an environment variable defined in envar.h
void set_env(char* env_var, char* val){
	strcpy(env_var, val);
}

//The first arg should be an environment variable defined in envar.h
void unset_env(char* env_var){
	env_var = NULL;
}

void change_dir(char* word){
	char* cwd = malloc(100);
    int success = chdir(word);
    getcwd(cwd, 100);
    if(success == 0)
    {
    	printf("\tChanged directory to %s\n", cwd);
    	set_env(PWD, cwd);
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
    int i = 0;

    while(i != alias_count)
    {
        char* name = alias_table[i].ali;
        char* word = alias_table[i].cmd;

        ++i;

        printf("\tAlias: %s \tCommand: %s\n", name, word);
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
}