/*
 * This header file declares environment variables for the shell
 */

int exitRequested;	/* Used by the shell loop to test for exit condition */
char* PATH;			/* The starting path variable*/
char* HOME;			/* The home path */
char* PWD; 			/* The current working directory */

 char* getLocalEnvironmentVariable(char* var_name);