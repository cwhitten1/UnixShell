#include "io_redir.h"

//Should be called from a forked process
//Returns 0 if redirect successful, -1 otherwise
int redirect_in(char* fn){
	if(fn == NULL)
		return -1;

	int fd = open(fn, O_RDONLY);

	if(fd == -1)
		return -1; 
	

	//printf("\tFile %s found!\n", fn);

	dup2(fd, 0);
	close(fd);

	return 0;
}

//Should be called from a forked process
//Returns 0 if redirect successful, -1 otherwise
int redirect_out(char* fn, int append){
	if(fn == NULL)
		return -1;
	
	int fd;
	printf("%d\n",append );
	if(append == 1)
		fd = open(fn, O_CREAT | O_WRONLY | O_APPEND,  S_IREAD | S_IWRITE);
	else
		fd = open(fn, O_CREAT | O_WRONLY | O_TRUNC,  S_IREAD | S_IWRITE);

	dup2(fd, 1);
	close(fd);
	return 0;
}

void reset_in(){

}
void reset_out(){

}
void reset(){

}