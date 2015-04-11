/*
 * This file is used to define the error handling struct that defines tokens for the user
 */

 #include <string.h>

 struct errorMsgs{
 	char* token;
 	char* msg;
 	char* syntax;
 };

 struct errorMsgs errTable[] = {
 								{"NUMBER"		,"a number"									,"no suggestions"}, 
 								{"TOKCD"		,"change directory command"					,"cd | cd WORD | alias WORD cd"}, 
 								{"TOKSETENV"	,"set environment variable command"			,"setenv WORD WORD | alias WORD setenv"},
                               	{"TOKCLEARENV"	,"clear environment variable command"		,"unsetenv WORD | alias WORD printenv"}, 
                               	{"TOKPRINTENV"	,"print environment variable command"		,"printenv | alias WORD printenv"}, 
                               	{"TOKALIAS"		,"shorthand for other command"				,"alias WORD (COMMAND) | alias WORD WORD"},
                               	{"TOKUNALIAS"	,"delete alias command"						,"unalias WORD"}, 
                               	{"TOKBYE"		,"exit shell command"						,"alias WORD bye | bye"}, 
                               	{"WORD"			,"a pseudo command"							,"setenv WORD WORD | alias WORD WORD | unsetenv WORD"},
                               	{"TOKENDEXP"	,"newline"									,"no suggestions"}
                               };
int errTable_size = 10;