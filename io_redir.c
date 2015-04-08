#include "io_redir.h"

void redirect_in(char* fn){
	int fd = open(fn, O_RDONLY);

	if(fd == -1)
	{
		printf("\tFile %s not readable or does not exist.\n", fn);
		return;
	}

	dup2(fd, 0);
	close(fd);
}
void redirect_out(char* fo){

}
void reset_in(){

}
void reset_out(){

}
void reset(){

}