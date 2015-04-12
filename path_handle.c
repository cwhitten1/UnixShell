#include "path_handle.h"

char* searchPathForFile(char* fn){

	//If the filename begins with a '/', check just fn and return
	if(fn[0] == '/')
	{
		if(doesFileExist(fn))
			return fn;
		else
			return NULL;
	}
	else if(fn[0] == '.' && fn[1] == '/')
	{
		//Get rid of '.'
		char* filename = strdup(fn);
		filename++;

		//Append current working directory
		char* pathname = strdup(PWD);
		strcat(pathname, filename);

		printf("%s\n", pathname);
		if(doesFileExist(pathname))
			return pathname;
		else
			return NULL;
	}

	//Otherwise we check the full PATH
	int charInd = 0;
	char* env_path = strdup(PATH);
	char* token;
	char* currPath;

	//Split the string based on delim
	token = strtok(env_path, ":");
 
	while(token != NULL)
	{
		//Create full pathname
		currPath = strdup(token);
		strcat(currPath, "/");
		strcat(currPath, fn);

		//perform file check
		int f_exists = doesFileExist(currPath);
		if(f_exists)
			return currPath;
			
		
		token = strtok(NULL, ":");
	}

	return NULL;
}

int doesFileExist(char* pathname){
	struct stat s;
	//printf("Checking path: %s\n", pathname);
	if(stat(pathname, &s) == 0 && S_ISREG(s.st_mode))
		return 1;
	else
		return 0;
}