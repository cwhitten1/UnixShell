/*
 * This file is used to define the 'alias' struct that associates "pseudo" commands with real commands
 */

 #include <string.h>
 #define MAX_SIZE 100

 struct alias{
 	char* ali;
 	char* cmd;
 };