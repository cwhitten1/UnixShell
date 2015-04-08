#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void redirect_in(char* fn);		/* Redirect stdin to file fn*/
void redirect_out(char* fo);	/* Redirect stdout to file fo*/
void reset_in();				/* Reset stdin to default*/
void reset_out();				/* Reset stdout to default*/
void reset();					/* Reset both to default*/