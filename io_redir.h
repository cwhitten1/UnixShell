#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define redir_in 1
#define redir_out 2
#define no_redir 3
int redirect_in(char* fn);		/* Redirect stdin to file fn*/
int redirect_out(char* fn, int append);	/* Redirect stdout to file fo*/
void reset_in();				/* Reset stdin to default*/
void reset_out();				/* Reset stdout to default*/
void reset();					/* Reset both to default*/