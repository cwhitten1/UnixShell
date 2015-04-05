#include "cmdcode.h"

void print_env(){
	int i=0;

    while(environ[i]){
      printf("%s\n\n", environ[i++]);
    }

	printf("\n\n\n\n");
}

void set_env(char* env_var, char* val){

    //Copy new value to local variable (if one exists i.e. PATH)
    char* env_var_local = getLocalEnvironmentVariable(env_var);
    if(env_var_local != NULL)
	   strcpy(env_var_local, val);

    //Modify process environment variable
    int success = setenv(env_var, val, 1); //Returns 0 on success, -1 otherwise
}

void unset_env(char* env_var){
    //Remove local copy
	char* env_var_local = getLocalEnvironmentVariable(env_var);
    if(env_var_local != NULL)
       env_var_local = NULL;

    //Modify process environment variable
    int success = unsetenv(env_var); //Returns 0 on success, -1 otherwise
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

