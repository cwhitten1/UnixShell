#include "cmdcode.h"

void print_env(){
	printf("PATH: %s\n", PATH);
	printf("HOME: %s\n", HOME);
	printf("PWD: %s\n", PWD);
	printf("\n\n\n\n");
}

//The first arg should be an environment variable defined in envar.h
void set_env(char* env_var, char* val){
	strcpy(env_var, val);
    //I think we may need to make a system call to setenv() as well
}

//The first arg should be an environment variable defined in envar.h
void unset_env(char* env_var){
	env_var = NULL;
     //I think we may need to make a system call to setenv() as well
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

