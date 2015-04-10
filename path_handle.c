#include "path_handle.h"

char* searchPathForFile(char* fn){
	int charInd = 0;
	char* env_path = strdup(PATH);
	char* token;
	char* resultPath;

	//Split the string based on delim
	token = strtok(env_path, ":");
 
	while(token != NULL)
	{
		//Create full pathname
		strcat(token, fn);

		//perform file check
		int f_exists = doesFileExist(token);
		if(f_exists)
		{
			resultPath = strdup(token);
			return resultPath;
		}	
		
		token = strtok(NULL, ":");
	}

	return NULL;
}

int doesFileExist(char* pathname){
	struct stat s;
	if(stat(pathname, &s) == 0 && S_ISREG(s.st_mode))
		return 1;
	else
		return 0;
}