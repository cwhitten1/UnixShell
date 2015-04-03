#include "cmdcode.h"

void print_env(){
	printf("PATH: %s\n", PATH);
	printf("HOME: %s\n", HOME);
	printf("PWD: %s\n", PWD);
	printf("\n\n\n\n");
}

void set_env(char* env_var, char* val){
	strcpy(env_var, val);
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
    free(cwd);       
}
