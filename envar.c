#include <string.h>
#include "envar.h"


char* getEnvironmentVariable(char* var_name){
	if(strcmp(var_name, "PATH") == 0)
		return PATH;
	else if(strcmp(var_name, "HOME") == 0)
		return HOME;
	else if(strcmp(var_name, "PWD") == 0)
		return PWD;
	else
		return NULL;
}